/**
 * @file GUIHandler.h
 * @brief GUI management using SFML
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
    Path currentPath_;                  ///< Current displayed path
    bool pathCalculated_;               ///< Whether path is calculated
    float zoomLevel_;                   ///< Current zoom level
    sf::Vector2f viewOffset_;           ///< View panning offset
    
    // UI constants
    static const int WINDOW_WIDTH = 1200;
    static const int WINDOW_HEIGHT = 800;
    static const int MARKER_RADIUS = 8;
    static const int INFO_PANEL_WIDTH = 300;
    
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
