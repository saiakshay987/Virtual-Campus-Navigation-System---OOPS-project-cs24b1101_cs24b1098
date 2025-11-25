/**
 * @file GUIHandler.h
 * @brief Declarations for the SFML-based GUI handler.
 *
 * Manages UI state, rendering and input for the Virtual Campus Navigator.
 */

#ifndef GUI_HANDLER_H
#define GUI_HANDLER_H

#include <SFML/Graphics.hpp>
#include "Navigator.h"
#include "Location.h"
#include "Path.h"
#include "CampusData.h"
#include <memory>
#include <string>
#include <vector>

/**
 * @class GUIHandler
 * @brief Manages the graphical user interface using SFML
 */
class GUIHandler {
private:
    sf::RenderWindow window_;           ///< Main window
    Navigator& navigator_;              ///< Reference to navigator
    sf::Font font_;                     ///< Font for text
    
    // UI state
    Location* selectedStart_;           ///< Selected start location
    Location* selectedEnd_;             ///< Selected end location
    // UI modes: Explore shows info on click, Navigation selects start/end
    enum class UIMode { Explore, Navigation };
    UIMode uiMode_;                      ///< Current UI mode
    Location* inspectedLocation_;       ///< Location selected in Explore mode
    std::vector<Location*> viaLocations_; ///< Ordered vias selected in Navigation mode
    // Screen-space rect of the toggle button (updated each frame in drawInfoPanel)
    sf::FloatRect toggleButtonScreenRect_;
    // Screen-space rects for via controls (up/down/remove) per via entry
    std::vector<sf::FloatRect> viaUpRects_;
    std::vector<sf::FloatRect> viaDownRects_;
    std::vector<sf::FloatRect> viaRemoveRects_;
    Path currentPath_;                  ///< Current displayed path
    bool pathCalculated_;               ///< Whether path is calculated
    float zoomLevel_;                   ///< Current zoom level
    sf::Vector2f viewOffset_;           ///< View panning offset
    
    // Pan/drag state
    bool isDragging_;                   ///< Whether currently dragging to pan
    sf::Vector2f dragStartPos_;         ///< World pos where drag started
    sf::Vector2f dragStartOffset_;      ///< viewOffset_ value when drag started
    
    // UI constants
    static const int WINDOW_WIDTH = 1200;
    static const int WINDOW_HEIGHT = 800;
    static const int MARKER_RADIUS = 8;
    static const int INFO_PANEL_WIDTH = 300;
    // Error message to display in info panel
    std::string lastErrorMsg_;
    
    /**
     * @brief Draw campus map background
     */
    void drawMap();
    
    /**
     * @brief Draw all building markers
     */
    void drawBuildings();
    
    /**
     * @brief Draw path connections
     */
    void drawPaths();
    
    /**
     * @brief Draw calculated route
     */
    void drawRoute();
    
    /**
     * @brief Draw info panel
     */
    void drawInfoPanel();
    
    /**
     * @brief Handle mouse click on building
     * @param mousePos Mouse position
     */
    void handleBuildingClick(sf::Vector2f mousePos);
    
    /**
     * @brief Convert location to screen position
     * @param loc Location pointer
     * @return Screen position
     */
    sf::Vector2f locationToScreen(Location* loc);
    
public:
    /**
     * @brief Constructor
     * @param navigator Navigator reference
     */
    explicit GUIHandler(Navigator& navigator);
    
    /**
     * @brief Initialize GUI
     * @return True if successful
     */
    bool initialize();
    
    /**
     * @brief Main event loop
     */
    void run();
    
    /**
     * @brief Handle events
     */
    void handleEvents();
    
    /**
     * @brief Update GUI
     */
    void update();
    
    /**
     * @brief Render GUI
     */
    void render();
};

#endif // GUI_HANDLER_H