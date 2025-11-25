/**
 * @file GUIHandler.cpp
 * @brief Implementation of the SFML-based GUI handler.
 *
 * Handles rendering, input processing, and UI controls for navigation
 * and explore modes.
 */

#include "GUIHandler.h"
#include "WalkingMode.h"
#include "CyclingMode.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>

// Constructor
GUIHandler::GUIHandler(Navigator& navigator)
    : navigator_(navigator),
      selectedStart_(nullptr),
      selectedEnd_(nullptr),
    uiMode_(UIMode::Navigation),
    inspectedLocation_(nullptr),
      pathCalculated_(false),
      zoomLevel_(1.0f),
      viewOffset_(0.0f, 0.0f),
      isDragging_(false),
      dragStartPos_(0.0f, 0.0f),
      dragStartOffset_(0.0f, 0.0f) {
}

// Initialize GUI
bool GUIHandler::initialize() {
    // Create window
    window_.create(sf::VideoMode(sf::Vector2u(static_cast<unsigned int>(WINDOW_WIDTH), static_cast<unsigned int>(WINDOW_HEIGHT))), "Virtual Campus Navigator - IIITDM Kancheepuram", sf::Style::Default);
    window_.setFramerateLimit(60);

    // Load font from common Windows font locations
    std::vector<std::string> fontPaths = {
        "C:\\Windows\\Fonts\\arial.ttf",
        "C:\\Windows\\Fonts\\Arial.ttf",
        "C:\\Windows\\Fonts\\segoeui.ttf",
        "C:\\Windows\\Fonts\\Segoe UI.ttf"
    };

    for (const auto& path : fontPaths) {
        if (font_.openFromFile(path)) {
            return true;
        }
    }

    // Fallback: try to load default bundled font if present
    if (font_.openFromFile("assets/arial.ttf")) {
        return true;
    }

    // If font not found, report warning but allow program to continue (text may not render)
    std::cerr << "Warning: Could not load any font. Text may not render correctly." << std::endl;
    return true;
}

// Main loop
void GUIHandler::run() {
    while (window_.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

// Handle events
void GUIHandler::handleEvents() {
    while (auto eventOpt = window_.pollEvent()) {
        const auto& event = *eventOpt;

        if (event.getIf<sf::Event::Closed>()) {
            window_.close();
            return;
        }

        if (const auto* mousePress = event.getIf<sf::Event::MouseButtonPressed>()) {
            sf::Vector2i pixelPos(mousePress->position.x, mousePress->position.y);
            sf::Vector2f pixelPosF(static_cast<float>(pixelPos.x), static_cast<float>(pixelPos.y));

            if (mousePress->button == sf::Mouse::Button::Left) {
                // Check toggle button first (screen-space)
                if (toggleButtonScreenRect_.contains(pixelPosF)) {
                    uiMode_ = (uiMode_ == UIMode::Explore) ? UIMode::Navigation : UIMode::Explore;
                    inspectedLocation_ = nullptr;
                    continue;
                }

                // Check via control buttons
                bool handledViaButton = false;
                for (size_t vi = 0; vi < viaUpRects_.size(); ++vi) {
                    if (viaUpRects_[vi].contains(pixelPosF)) {
                        if (vi > 0) std::swap(viaLocations_[vi], viaLocations_[vi-1]);
                        handledViaButton = true;
                        break;
                    }
                    if (viaDownRects_[vi].contains(pixelPosF)) {
                        if (vi + 1 < viaLocations_.size()) std::swap(viaLocations_[vi], viaLocations_[vi+1]);
                        handledViaButton = true;
                        break;
                    }
                    if (viaRemoveRects_[vi].contains(pixelPosF)) {
                        viaLocations_.erase(viaLocations_.begin() + vi);
                        handledViaButton = true;
                        break;
                    }
                }

                if (handledViaButton) {
                    if (selectedStart_ && selectedEnd_) {
                        try {
                            if (!viaLocations_.empty()) currentPath_ = navigator_.findPath(selectedStart_, selectedEnd_, viaLocations_);
                            else currentPath_ = navigator_.findPath(selectedStart_, selectedEnd_);
                            pathCalculated_ = true;
                            lastErrorMsg_.clear();
                        } catch (const std::exception& e) {
                            lastErrorMsg_ = e.what();
                            pathCalculated_ = false;
                        }
                    }
                    continue;
                }

                // Map to world and start drag/selection
                sf::View worldView = window_.getDefaultView();
                worldView.setSize(window_.getDefaultView().getSize() / zoomLevel_);
                worldView.move(viewOffset_);
                sf::Vector2f worldPos = window_.mapPixelToCoords(pixelPos, worldView);

                isDragging_ = true;
                dragStartPos_ = worldPos;
                dragStartOffset_ = viewOffset_;

                auto handCursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Hand);
                if (handCursor.has_value()) window_.setMouseCursor(handCursor.value());

                handleBuildingClick(worldPos);

            } else if (mousePress->button == sf::Mouse::Button::Right) {
                // Right-click: toggle via selection on building (Navigation mode only)
                sf::View worldView = window_.getDefaultView();
                worldView.setSize(window_.getDefaultView().getSize() / zoomLevel_);
                worldView.move(viewOffset_);
                sf::Vector2f worldPos = window_.mapPixelToCoords(pixelPos, worldView);

                std::vector<Location*> locations = navigator_.getAllLocations();
                Location* clicked = nullptr;
                for (Location* loc : locations) {
                    sf::Vector2f screenPos = locationToScreen(loc);
                    float dx = worldPos.x - screenPos.x;
                    float dy = worldPos.y - screenPos.y;
                    float distSq = dx*dx + dy*dy;
                    if (distSq <= (MARKER_RADIUS * 1.5f) * (MARKER_RADIUS * 1.5f)) {
                        clicked = loc;
                        break;
                    }
                }

                if (clicked != nullptr && uiMode_ == UIMode::Navigation) {
                    if (clicked == selectedStart_ || clicked == selectedEnd_) {
                        lastErrorMsg_ = "Cannot mark Start/End as via";
                        std::cerr << lastErrorMsg_ << std::endl;
                    } else {
                        auto it = std::find(viaLocations_.begin(), viaLocations_.end(), clicked);
                        if (it != viaLocations_.end()) viaLocations_.erase(it);
                        else viaLocations_.push_back(clicked);

                        if (selectedStart_ && selectedEnd_) {
                            try {
                                currentPath_ = navigator_.findPath(selectedStart_, selectedEnd_, viaLocations_);
                                pathCalculated_ = true;
                                lastErrorMsg_.clear();
                            } catch (const std::exception& e) {
                                lastErrorMsg_ = e.what();
                                std::cerr << "Error calculating path with vias: " << e.what() << std::endl;
                                pathCalculated_ = false;
                            }
                        }
                    }
                }
            }
        }

        // Handle mouse button release (stop dragging)
        if (const auto* mouseRelease = event.getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseRelease->button == sf::Mouse::Button::Left) {
                isDragging_ = false;
                auto arrowCursor = sf::Cursor::createFromSystem(sf::Cursor::Type::Arrow);
                if (arrowCursor.has_value()) window_.setMouseCursor(arrowCursor.value());
            }
        }

        // Handle mouse move (for panning)
        if (const auto* mouseMove = event.getIf<sf::Event::MouseMoved>()) {
            if (isDragging_) {
                sf::View worldView = window_.getDefaultView();
                worldView.setSize(window_.getDefaultView().getSize() / zoomLevel_);
                worldView.move(viewOffset_);

                sf::Vector2i pixelPos(mouseMove->position.x, mouseMove->position.y);
                sf::Vector2f currentWorldPos = window_.mapPixelToCoords(pixelPos, worldView);

                sf::Vector2f delta = dragStartPos_ - currentWorldPos;
                viewOffset_ = dragStartOffset_ + delta;
            }
        }

        // Handle zoom
        if (const auto* wheelScroll = event.getIf<sf::Event::MouseWheelScrolled>()) {
            if (wheelScroll->delta > 0) zoomLevel_ *= 1.1f; else zoomLevel_ /= 1.1f;
            if (zoomLevel_ < 0.5f) zoomLevel_ = 0.5f;
            if (zoomLevel_ > 3.0f) zoomLevel_ = 3.0f;
        }

        // Handle keyboard
        if (const auto* keyPress = event.getIf<sf::Event::KeyPressed>()) {
            if (keyPress->code == sf::Keyboard::Key::E) { uiMode_ = UIMode::Explore; inspectedLocation_ = nullptr; }
            if (keyPress->code == sf::Keyboard::Key::N) { uiMode_ = UIMode::Navigation; inspectedLocation_ = nullptr; }

            if (keyPress->code == sf::Keyboard::Key::W) {
                navigator_.setNavigationMode(std::make_shared<WalkingMode>());
                if (pathCalculated_) {
                    try {
                        if (!viaLocations_.empty()) currentPath_ = navigator_.findPath(selectedStart_, selectedEnd_, viaLocations_);
                        else currentPath_ = navigator_.findPath(selectedStart_, selectedEnd_);
                    } catch (const std::exception& e) { std::cerr << "Error: " << e.what() << std::endl; }
                }
            }

            if (keyPress->code == sf::Keyboard::Key::C) {
                navigator_.setNavigationMode(std::make_shared<CyclingMode>());
                if (pathCalculated_) {
                    try {
                        if (!viaLocations_.empty()) currentPath_ = navigator_.findPath(selectedStart_, selectedEnd_, viaLocations_);
                        else currentPath_ = navigator_.findPath(selectedStart_, selectedEnd_);
                    } catch (const std::exception& e) { std::cerr << "Error: " << e.what() << std::endl; }
                }
            }

            if (keyPress->code == sf::Keyboard::Key::Escape) {
                selectedStart_ = nullptr; selectedEnd_ = nullptr; pathCalculated_ = false;
            }
        }
    }
}

// Update
void GUIHandler::update() {
    // Update logic if needed
}

// Render
void GUIHandler::render() {
    window_.clear(sf::Color(20, 20, 30)); // Dark blue-black background
    
    // Apply zoom and pan
    sf::View view = window_.getDefaultView();
    // Adjust view center and size for zoom
    view.setSize(window_.getDefaultView().getSize() / zoomLevel_);
    view.move(viewOffset_); // Apply panning
    window_.setView(view);
    
    // Draw components in world space
    drawMap();
    drawPaths();
    if (pathCalculated_) {
        drawRoute();
    }
    drawBuildings();
    
    // Reset view for UI elements (screen space)
    window_.setView(window_.getDefaultView());
    drawInfoPanel();
    
    window_.display();
}

// Draw map background
void GUIHandler::drawMap() {
    // Dark theme: subtle dark grid
    sf::Color gridColor(60, 60, 60);
    
    // These grid lines are fixed to the window, not the world.
    // For a world grid, you'd need to calculate bounds based on the view.
    for (int x = 0; x < WINDOW_WIDTH - INFO_PANEL_WIDTH; x += 50) {
        sf::RectangleShape line(sf::Vector2f(1, WINDOW_HEIGHT));
        line.setPosition(sf::Vector2f(static_cast<float>(x), 0.0f));
        line.setFillColor(gridColor);
        window_.draw(line);
    }
    
    for (int y = 0; y < WINDOW_HEIGHT; y += 50) {
        sf::RectangleShape line(sf::Vector2f(WINDOW_WIDTH - INFO_PANEL_WIDTH, 1));
        line.setPosition(sf::Vector2f(0.0f, static_cast<float>(y)));
        line.setFillColor(gridColor);
        window_.draw(line);
    }
}

// Draw buildings
void GUIHandler::drawBuildings() {
    std::vector<Location*> locations = navigator_.getAllLocations();
    
    for (Location* loc : locations) {
        sf::Vector2f screenPos = locationToScreen(loc);
        
        // Draw marker circle
        sf::CircleShape marker(MARKER_RADIUS);
        marker.setPosition(sf::Vector2f(screenPos.x - MARKER_RADIUS, screenPos.y - MARKER_RADIUS));
        
        // Color based on selection/mode
        if (loc == selectedStart_) {
            marker.setFillColor(sf::Color::Green);
            marker.setOutlineColor(sf::Color::White);
            marker.setOutlineThickness(2);
        } else if (loc == selectedEnd_) {
            marker.setFillColor(sf::Color::Red);
            marker.setOutlineColor(sf::Color::White);
            marker.setOutlineThickness(2);
        } else if (std::find(viaLocations_.begin(), viaLocations_.end(), loc) != viaLocations_.end() && uiMode_ == UIMode::Navigation) {
            marker.setFillColor(sf::Color(200, 100, 200)); // magenta for via
            marker.setOutlineColor(sf::Color::White);
            marker.setOutlineThickness(3);
        } else if (loc == inspectedLocation_ && uiMode_ == UIMode::Explore) {
            marker.setFillColor(sf::Color(255, 220, 100)); // warm highlight
            marker.setOutlineColor(sf::Color::Yellow);
            marker.setOutlineThickness(3);
        } else {
            marker.setFillColor(sf::Color::Cyan);
            marker.setOutlineColor(sf::Color::White);
            marker.setOutlineThickness(2);
        }
        
        window_.draw(marker);
        
        // Draw building name label with light text
        // Skip labels for turn/waypoint nodes. We support two markers:
        // - Name starts with "turn_" (convention)
        // - Description equals "[hidden]" (explicit hide flag)
        const std::string name = loc->getName();
        const std::string desc = loc->getDescription();
        bool hideLabel = false;
        if (!name.empty()) {
            if (name.rfind("turn_", 0) == 0) hideLabel = true;
        }
        if (!desc.empty() && desc == "[hidden]") hideLabel = true;

        if (!hideLabel) {
            sf::Text label(font_, loc->getName(), 11u);
            label.setFillColor(sf::Color::White);
            label.setPosition(sf::Vector2f(screenPos.x + MARKER_RADIUS + 8, screenPos.y - 10));
            window_.draw(label);
        }

        // If this location is a via (and we're in Navigation mode), draw an ordered badge
        if (uiMode_ == UIMode::Navigation) {
            auto it = std::find(viaLocations_.begin(), viaLocations_.end(), loc);
            if (it != viaLocations_.end()) {
                int idx = static_cast<int>(std::distance(viaLocations_.begin(), it));
                float br = 12.0f; // badge radius
                // Badge center position (slightly left-top of marker, offset by badge index to prevent overlap)
                float badgeCx = screenPos.x - MARKER_RADIUS - 12.0f - (idx * 28.0f);
                float badgeCy = screenPos.y - MARKER_RADIUS - 12.0f;

                // Draw circle badge
                sf::CircleShape badge(br);
                badge.setPosition(sf::Vector2f(badgeCx - br, badgeCy - br));
                badge.setFillColor(sf::Color(200, 100, 200));
                badge.setOutlineColor(sf::Color::White);
                badge.setOutlineThickness(1);
                window_.draw(badge);

                // Draw number centered in badge
                std::string numStr = std::to_string(idx + 1);
                sf::Text numText(font_, numStr, 12u);
                numText.setFillColor(sf::Color::White);
                numText.setPosition(sf::Vector2f(badgeCx - 5.0f, badgeCy - 8.0f));
                window_.draw(numText);
            }
        }
    }
}

// Draw path connections
void GUIHandler::drawPaths() {
    // Draw only the actual accessible paths from the graph
    std::vector<Location*> locations = navigator_.getAllLocations();
    
    sf::Color pathColor(100, 150, 200, 180);
    
    for (Location* loc : locations) {
        sf::Vector2f pos1 = locationToScreen(loc);
        
        // Get neighbors from graph (actual connections)
        auto neighbors = navigator_.getGraph().getNeighbors(loc);
        
        for (const auto& edge : neighbors) {
            Location* neighbor = edge.destination;
            sf::Vector2f pos2 = locationToScreen(neighbor);
            
            float dx = pos2.x - pos1.x;
            float dy = pos2.y - pos1.y;
            float dist = std::sqrt(dx * dx + dy * dy);
            
            if (dist > 0.0f) {
                // Draw line as thin rectangle
                sf::RectangleShape connectionLine(sf::Vector2f(dist, 2.0f));
                connectionLine.setPosition(pos1);
                float angle = std::atan2(dy, dx) * 180.0f / 3.14159265f;
                connectionLine.setRotation(sf::degrees(angle));
                connectionLine.setFillColor(pathColor);
                window_.draw(connectionLine);
            }
        }
    }
}

// Draw calculated route
void GUIHandler::drawRoute() {
    if (currentPath_.empty()) return;
    
    std::vector<Location*> pathLocs = currentPath_.getLocations();
    
    for (size_t i = 0; i < pathLocs.size() - 1; ++i) {
        sf::Vector2f start = locationToScreen(pathLocs[i]);
        sf::Vector2f end = locationToScreen(pathLocs[i + 1]);
        
        // Draw thick line using RectangleShape for visibility
        sf::Vector2f direction = end - start;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        sf::RectangleShape line(sf::Vector2f(distance, 2.0f));
        line.setPosition(start);
        float angleDeg = std::atan2(direction.y, direction.x) * 180.0f / 3.14159265f;
        line.setRotation(sf::degrees(angleDeg));
        line.setFillColor(sf::Color::Red);
        window_.draw(line);
    }
}

// Draw info panel
void GUIHandler::drawInfoPanel() {
    // Draw panel background - dark with slight transparency
    sf::RectangleShape panel(sf::Vector2f(INFO_PANEL_WIDTH, WINDOW_HEIGHT));
    panel.setPosition(sf::Vector2f(static_cast<float>(WINDOW_WIDTH - INFO_PANEL_WIDTH), 0.0f));
    panel.setFillColor(sf::Color(30, 30, 40, 240));
    window_.draw(panel);
    
    // Draw panel border
    sf::RectangleShape panelBorder(sf::Vector2f(INFO_PANEL_WIDTH, WINDOW_HEIGHT));
    panelBorder.setPosition(sf::Vector2f(static_cast<float>(WINDOW_WIDTH - INFO_PANEL_WIDTH), 0.0f));
    panelBorder.setFillColor(sf::Color::Transparent);
    panelBorder.setOutlineColor(sf::Color(100, 150, 200));
    panelBorder.setOutlineThickness(2);
    window_.draw(panelBorder);
    
    // Draw info text with light color
    sf::Text info(font_, "", 13u);
    info.setFillColor(sf::Color::White);
    info.setPosition(sf::Vector2f(static_cast<float>(WINDOW_WIDTH - INFO_PANEL_WIDTH + 10), 10.0f));
    
    std::stringstream ss;
    // **FIXED:** Changed \\n to \n for proper newlines
    ss << "Virtual Campus Navigator\n";
    ss << "IIITDM Kancheepuram\n\n";

    // UI mode (Explore / Navigation)
    ss << "UI Mode: " << (uiMode_ == UIMode::Explore ? "Explore" : "Navigation") << "\n";
    ss << "(Press E to Explore, N for Navigation)\n\n";

    // Always show the navigation mode used for route calculation
    ss << "Nav Mode: " << navigator_.getNavigationMode()->getModeName() << "\n";
    ss << "Press W for Walking\n";
    ss << "Press C for Cycling\n\n";

    if (uiMode_ == UIMode::Explore) {
        if (inspectedLocation_) {
            ss << "Name: " << inspectedLocation_->getName() << "\n";
            ss << "ID: " << inspectedLocation_->getId() << "\n";
            ss << "Coords: " << std::fixed << std::setprecision(6)
               << inspectedLocation_->getLatitude() << ", "
               << inspectedLocation_->getLongitude() << "\n\n";
            ss << "Description:\n" << inspectedLocation_->getDescription() << "\n";
        } else {
            ss << "Explore mode active. Click a building to view details.\n";
        }
    } else {
        // Navigation UI
        if (selectedStart_) {
            ss << "Start: " << selectedStart_->getName() << "\n";
        } else {
            ss << "Start: (Click a building)\n";
        }

        if (selectedEnd_) {
            ss << "End: " << selectedEnd_->getName() << "\n";
        } else {
            ss << "End: (Click a building)\n";
        }

        // List any via waypoints
        if (!viaLocations_.empty()) {
            ss << "\nVia: ";
            for (size_t i = 0; i < viaLocations_.size(); ++i) {
                ss << viaLocations_[i]->getName();
                if (i + 1 < viaLocations_.size()) ss << ", ";
            }
            ss << "\n";
        }
    }
    
    info.setString(ss.str());
    window_.draw(info);

    // Draw error message if present
    if (!lastErrorMsg_.empty()) {
        sf::Text errText(font_, lastErrorMsg_, 12u);
        errText.setFillColor(sf::Color::Red);
        errText.setPosition(sf::Vector2f(static_cast<float>(WINDOW_WIDTH - INFO_PANEL_WIDTH + 10), 200.0f));
        window_.draw(errText);
    }

    // Draw via list controls (up/down/remove) when in Navigation mode
    viaUpRects_.clear();
    viaDownRects_.clear();
    viaRemoveRects_.clear();

    float listX = static_cast<float>(WINDOW_WIDTH - INFO_PANEL_WIDTH + 10);
    float listY = 220.0f;
    float entryH = 35.0f;
    if (uiMode_ == UIMode::Navigation && !viaLocations_.empty()) {
        for (size_t i = 0; i < viaLocations_.size(); ++i) {
            Location* v = viaLocations_[i];
            // Draw via name
            sf::Text vText(font_, std::to_string(i+1) + ". " + v->getName(), 12u);
            vText.setFillColor(sf::Color::White);
            vText.setPosition(sf::Vector2f(listX + 6.0f, listY + i * entryH));
            window_.draw(vText);

            // Buttons positions
            float bx = listX + INFO_PANEL_WIDTH - 140.0f;
            float by = listY + i * entryH;
            float bw = 40.0f;
            float bh = 22.0f;

            // Up button
            sf::RectangleShape upBtn(sf::Vector2f(bw, bh));
            upBtn.setPosition(sf::Vector2f(bx, by));
            upBtn.setFillColor(i == 0 ? sf::Color(80,80,80) : sf::Color(90,140,180));
            upBtn.setOutlineColor(sf::Color::White);
            upBtn.setOutlineThickness(1);
            window_.draw(upBtn);
            sf::Text upLabel(font_, "u", 14u);
            upLabel.setFillColor(sf::Color::White);
            upLabel.setPosition(sf::Vector2f(bx + 12.0f, by - 1.0f));
            window_.draw(upLabel);
            viaUpRects_.push_back(sf::FloatRect(sf::Vector2f(bx, by), sf::Vector2f(bw, bh)));

            // Down button
            sf::RectangleShape downBtn(sf::Vector2f(bw, bh));
            downBtn.setPosition(sf::Vector2f(bx + bw + 8.0f, by));
            downBtn.setFillColor(i + 1 == viaLocations_.size() ? sf::Color(80,80,80) : sf::Color(90,140,180));
            downBtn.setOutlineColor(sf::Color::White);
            downBtn.setOutlineThickness(1);
            window_.draw(downBtn);
            sf::Text downLabel(font_, "d", 14u);
            downLabel.setFillColor(sf::Color::White);
            downLabel.setPosition(sf::Vector2f(bx + bw + 16.0f, by - 1.0f));
            window_.draw(downLabel);
            viaDownRects_.push_back(sf::FloatRect(sf::Vector2f(bx + bw + 8.0f, by), sf::Vector2f(bw, bh)));

            // Remove button
            sf::RectangleShape remBtn(sf::Vector2f(bw, bh));
            remBtn.setPosition(sf::Vector2f(bx + 2*(bw + 8.0f), by));
            remBtn.setFillColor(sf::Color(180,80,90));
            remBtn.setOutlineColor(sf::Color::White);
            remBtn.setOutlineThickness(1);
            window_.draw(remBtn);
            sf::Text remLabel(font_, "Del", 14u);
            remLabel.setFillColor(sf::Color::White);
            remLabel.setPosition(sf::Vector2f(bx + 2*(bw + 8.0f) + 6.0f, by - 1.0f));
            window_.draw(remLabel);
            viaRemoveRects_.push_back(sf::FloatRect(sf::Vector2f(bx + 2*(bw + 8.0f), by), sf::Vector2f(bw, bh)));
        }
    }

    // Draw distance and time even when there are no vias (if a path was calculated)
    if (uiMode_ == UIMode::Navigation && pathCalculated_) {
        float distY = listY + viaLocations_.size() * entryH + 10.0f;
        sf::Text distText(font_, "Distance: " + std::to_string(static_cast<int>(currentPath_.getTotalDistance())) + "m", 12u);
        distText.setFillColor(sf::Color::White);
        distText.setPosition(sf::Vector2f(listX, distY));
        window_.draw(distText);

        sf::Text timeText(font_, "Time: " + std::to_string(static_cast<int>(navigator_.getEstimatedTime())) + " min", 12u);
        timeText.setFillColor(sf::Color::White);
        timeText.setPosition(sf::Vector2f(listX, distY + 20.0f));
        window_.draw(timeText);
    }

    // Draw toggle button (screen space)
    float bx = static_cast<float>(WINDOW_WIDTH - INFO_PANEL_WIDTH + 10);
    float by = 60.0f;
    float bw = static_cast<float>(INFO_PANEL_WIDTH - 20);
    float bh = 30.0f;

    // Update stored button rect for event handling (SFML Rect accepts vectors)
    toggleButtonScreenRect_ = sf::FloatRect(sf::Vector2f(bx, by), sf::Vector2f(bw, bh));

    // Hover detection using current mouse position in window pixels
    sf::Vector2i mousePixel = sf::Mouse::getPosition(window_);
    bool hover = toggleButtonScreenRect_.contains(sf::Vector2f(static_cast<float>(mousePixel.x), static_cast<float>(mousePixel.y)));

    sf::RectangleShape btn(sf::Vector2f(bw, bh));
    btn.setPosition(sf::Vector2f(bx, by));
    btn.setFillColor(hover ? sf::Color(80, 120, 160) : sf::Color(60, 90, 120));
    btn.setOutlineColor(sf::Color::White);
    btn.setOutlineThickness(1);
    window_.draw(btn);

    // Button label: show action to switch to the other mode
    std::string btnLabel = (uiMode_ == UIMode::Explore) ? "Switch to Navigation (N)" : "Switch to Explore (E)";
    sf::Text btnText(font_, btnLabel, 14u);
    btnText.setFillColor(sf::Color::White);
    // center text vertically and add small left padding
    btnText.setPosition(sf::Vector2f(bx + 8.0f, by + (bh - 14.0f) / 2.0f - 1.0f));
    window_.draw(btnText);
}

// Handle building click
void GUIHandler::handleBuildingClick(sf::Vector2f worldMousePos) {
    // This function now receives world coordinates,
    // so no need to adjust for view here.
    
    std::vector<Location*> locations = navigator_.getAllLocations();
    
    for (Location* loc : locations) {
        sf::Vector2f screenPos = locationToScreen(loc);
        
        // Check if click is within marker radius
        float dx = worldMousePos.x - screenPos.x;
        float dy = worldMousePos.y - screenPos.y;
        float distSq = dx * dx + dy * dy;
        
        // Use a slighly larger radius for easier clicking
        if (distSq <= (MARKER_RADIUS * 1.5f) * (MARKER_RADIUS * 1.5f)) {
            // Building clicked
            if (uiMode_ == UIMode::Explore) {
                // In explore mode, clicking a building shows its details in the side panel
                inspectedLocation_ = loc;
            } else {
                // Navigation mode: select start/end and calculate path as before
                if (selectedStart_ == nullptr) {
                    selectedStart_ = loc;
                } else if (selectedEnd_ == nullptr && loc != selectedStart_) {
                    selectedEnd_ = loc;

                    // Calculate path (include vias if present)
                    try {
                        if (!viaLocations_.empty()) {
                            currentPath_ = navigator_.findPath(selectedStart_, selectedEnd_, viaLocations_);
                        } else {
                            currentPath_ = navigator_.findPath(selectedStart_, selectedEnd_);
                        }
                        pathCalculated_ = true;
                        lastErrorMsg_.clear();
                    } catch (const std::exception& e) {
                        std::cerr << "Error calculating path: " << e.what() << std::endl;
                        lastErrorMsg_ = e.what();
                        pathCalculated_ = false;
                    }
                } else {
                    // Reset selection
                    selectedStart_ = loc;
                    selectedEnd_ = nullptr;
                    pathCalculated_ = false;
                }
            }

            break; // Stop after finding the first clicked building
        }
    }
}

// Convert location to screen position
sf::Vector2f GUIHandler::locationToScreen(Location* loc) {
    // This converts GPS to world coordinates.
    // The view transform handles converting world to screen.
    auto worldCoords = CampusData::gpsToScreen(
        loc->getLatitude(),
        loc->getLongitude(),
        WINDOW_WIDTH - INFO_PANEL_WIDTH,
        WINDOW_HEIGHT
    );
    
    return sf::Vector2f(static_cast<float>(worldCoords.first), 
                        static_cast<float>(worldCoords.second));
}