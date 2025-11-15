/**
 * @file GUIHandler.cpp
 * @brief Implementation of SFML GUI
 */

#include "GUIHandler.h"
#include "WalkingMode.h"
#include "CyclingMode.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// Constructor
GUIHandler::GUIHandler(Navigator& navigator)
    : navigator_(navigator),
      selectedStart_(nullptr),
      selectedEnd_(nullptr),
      pathCalculated_(false),
      zoomLevel_(1.0f),
      viewOffset_(0.0f, 0.0f) {
}

// Initialize GUI
bool GUIHandler::initialize() {
    // Create window
    window_.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                   "Virtual Campus Navigator - IIITDM Kancheepuram",
                   sf::Style::Default);
    window_.setFramerateLimit(60);
    
    // Load font (use default system font or provide a font file)
    // For this example, we'll use SFML's default font
    // In production, load a TTF font file:
    // if (!font_.loadFromFile("arial.ttf")) {
    //     return false;
    // }
    
    return true;
}

// Main event loop
void GUIHandler::run() {
    while (window_.isOpen()) {
        handleEvents();
        update();
        render();
    }
}

// Handle events
void GUIHandler::handleEvents() {
    sf::Event event;
    while (window_.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window_.close();
        }
        
        // Handle mouse click
        if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
                handleBuildingClick(mousePos);
            }
        }
        
        // Handle zoom
        if (event.type == sf::Event::MouseWheelScrolled) {
            if (event.mouseWheelScroll.delta > 0) {
                zoomLevel_ *= 1.1f;
            } else {
                zoomLevel_ /= 1.1f;
            }
            // Clamp zoom level
            if (zoomLevel_ < 0.5f) zoomLevel_ = 0.5f;
            if (zoomLevel_ > 3.0f) zoomLevel_ = 3.0f;
        }
        
        // Handle keyboard
        if (event.type == sf::Event::KeyPressed) {
            // Switch navigation mode
            if (event.key.code == sf::Keyboard::W) {
                navigator_.setNavigationMode(std::make_shared<WalkingMode>());
                if (pathCalculated_) {
                    // Recalculate with new mode
                    try {
                        currentPath_ = navigator_.findPath(selectedStart_, selectedEnd_);
                    } catch (const std::exception& e) {
                        std::cerr << "Error: " << e.what() << std::endl;
                    }
                }
            }
            if (event.key.code == sf::Keyboard::C) {
                navigator_.setNavigationMode(std::make_shared<CyclingMode>());
                if (pathCalculated_) {
                    try {
                        currentPath_ = navigator_.findPath(selectedStart_, selectedEnd_);
                    } catch (const std::exception& e) {
                        std::cerr << "Error: " << e.what() << std::endl;
                    }
                }
            }
            
            // Clear selection
            if (event.key.code == sf::Keyboard::Escape) {
                selectedStart_ = nullptr;
                selectedEnd_ = nullptr;
                pathCalculated_ = false;
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
    window_.clear(sf::Color(240, 240, 240)); // Light gray background
    
    // Apply zoom and pan
    sf::View view = window_.getDefaultView();
    view.zoom(1.0f / zoomLevel_);
    view.move(viewOffset_);
    window_.setView(view);
    
    // Draw components
    drawMap();
    drawPaths();
    if (pathCalculated_) {
        drawRoute();
    }
    drawBuildings();
    
    // Reset view for UI elements
    window_.setView(window_.getDefaultView());
    drawInfoPanel();
    
    window_.display();
}

// Draw map background
void GUIHandler::drawMap() {
    // Draw grid lines
    sf::Color gridColor(200, 200, 200);
    
    for (int x = 0; x < WINDOW_WIDTH; x += 50) {
        sf::RectangleShape line(sf::Vector2f(1, WINDOW_HEIGHT));
        line.setPosition(x, 0);
        line.setFillColor(gridColor);
        window_.draw(line);
    }
    
    for (int y = 0; y < WINDOW_HEIGHT; y += 50) {
        sf::RectangleShape line(sf::Vector2f(WINDOW_WIDTH, 1));
        line.setPosition(0, y);
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
        marker.setPosition(screenPos.x - MARKER_RADIUS, screenPos.y - MARKER_RADIUS);
        
        // Color based on selection
        if (loc == selectedStart_) {
            marker.setFillColor(sf::Color::Green);
        } else if (loc == selectedEnd_) {
            marker.setFillColor(sf::Color::Red);
        } else {
            marker.setFillColor(sf::Color::Blue);
        }
        marker.setOutlineColor(sf::Color::Black);
        marker.setOutlineThickness(2);
        
        window_.draw(marker);
        
        // Draw label
        sf::Text label;
        label.setFont(font_);
        label.setString(loc->getName());
        label.setCharacterSize(12);
        label.setFillColor(sf::Color::Black);
        label.setPosition(screenPos.x + MARKER_RADIUS + 5, screenPos.y - 6);
        
        window_.draw(label);
    }
}

// Draw path connections
void GUIHandler::drawPaths() {
    // Draw edges from graph
    std::vector<Location*> locations = navigator_.getAllLocations();
    
    for (Location* loc : locations) {
        sf::Vector2f startPos = locationToScreen(loc);
        
        // This is simplified - in full implementation, iterate through graph edges
        // For now, draw connections based on CampusData
    }
}

// Draw calculated route
void GUIHandler::drawRoute() {
    if (currentPath_.empty()) return;
    
    std::vector<Location*> pathLocs = currentPath_.getLocations();
    
    for (size_t i = 0; i < pathLocs.size() - 1; ++i) {
        sf::Vector2f start = locationToScreen(pathLocs[i]);
        sf::Vector2f end = locationToScreen(pathLocs[i + 1]);
        
        // Draw line
        sf::Vertex line[] = {
            sf::Vertex(start, sf::Color::Red),
            sf::Vertex(end, sf::Color::Red)
        };
        window_.draw(line, 2, sf::Lines);
        
        // Draw arrow or direction indicator
        // (Simplified - full implementation would draw proper arrows)
    }
}

// Draw info panel
void GUIHandler::drawInfoPanel() {
    // Draw panel background
    sf::RectangleShape panel(sf::Vector2f(INFO_PANEL_WIDTH, WINDOW_HEIGHT));
    panel.setPosition(WINDOW_WIDTH - INFO_PANEL_WIDTH, 0);
    panel.setFillColor(sf::Color(50, 50, 50, 200));
    window_.draw(panel);
    
    // Draw info text
    sf::Text info;
    info.setFont(font_);
    info.setCharacterSize(14);
    info.setFillColor(sf::Color::White);
    info.setPosition(WINDOW_WIDTH - INFO_PANEL_WIDTH + 10, 10);
    
    std::stringstream ss;
    ss << "Virtual Campus Navigator\\n";
    ss << "IIITDM Kancheepuram\\n\\n";
    
    ss << "Mode: " << navigator_.getNavigationMode()->getModeName() << "\\n";
    ss << "Press W for Walking\\n";
    ss << "Press C for Cycling\\n\\n";
    
    if (selectedStart_) {
        ss << "Start: " << selectedStart_->getName() << "\\n";
    } else {
        ss << "Start: (Click a building)\\n";
    }
    
    if (selectedEnd_) {
        ss << "End: " << selectedEnd_->getName() << "\\n";
    } else {
        ss << "End: (Click a building)\\n";
    }
    
    if (pathCalculated_) {
        ss << "\\nDistance: " << std::fixed << std::setprecision(2) 
           << currentPath_.getTotalDistance() << "m\\n";
        ss << "Time: " << std::fixed << std::setprecision(2) 
           << navigator_.getEstimatedTime() << " min\\n";
    }
    
    info.setString(ss.str());
    window_.draw(info);
}

// Handle building click
void GUIHandler::handleBuildingClick(sf::Vector2f mousePos) {
    // Convert mouse position accounting for zoom and pan
    // (Simplified - full implementation would handle view transforms)
    
    std::vector<Location*> locations = navigator_.getAllLocations();
    
    for (Location* loc : locations) {
        sf::Vector2f screenPos = locationToScreen(loc);
        
        // Check if click is within marker radius
        float dx = mousePos.x - screenPos.x;
        float dy = mousePos.y - screenPos.y;
        float distSq = dx * dx + dy * dy;
        
        if (distSq <= MARKER_RADIUS * MARKER_RADIUS * 2) {
            // Building clicked
            if (selectedStart_ == nullptr) {
                selectedStart_ = loc;
            } else if (selectedEnd_ == nullptr && loc != selectedStart_) {
                selectedEnd_ = loc;
                
                // Calculate path
                try {
                    currentPath_ = navigator_.findPath(selectedStart_, selectedEnd_);
                    pathCalculated_ = true;
                } catch (const std::exception& e) {
                    std::cerr << "Error calculating path: " << e.what() << std::endl;
                    pathCalculated_ = false;
                }
            } else {
                // Reset selection
                selectedStart_ = loc;
                selectedEnd_ = nullptr;
                pathCalculated_ = false;
            }
            
            break;
        }
    }
}

// Convert location to screen position
sf::Vector2f GUIHandler::locationToScreen(Location* loc) {
    auto screenCoords = CampusData::gpsToScreen(
        loc->getLatitude(),
        loc->getLongitude(),
        WINDOW_WIDTH - INFO_PANEL_WIDTH,
        WINDOW_HEIGHT
    );
    
    return sf::Vector2f(screenCoords.first, screenCoords.second);
}
