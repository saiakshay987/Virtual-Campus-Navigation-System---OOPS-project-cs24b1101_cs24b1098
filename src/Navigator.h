/**
 * @file Navigator.h
 * @brief Navigator class demonstrating ABSTRACTION and implementing Dijkstra's algorithm
 * 
 * OOP Concept: Abstraction
 * - Hides complex Dijkstra implementation
 * - Provides simple public interface
 * - Includes exception handling
 */

#ifndef NAVIGATOR_H
#define NAVIGATOR_H

#include "Location.h"
#include "Path.h"

/**
 * @file Navigator.h
 * @brief Navigator class declaration and pathfinding interface.
 *
 * Provides methods to initialize the campus graph, set navigation mode,
 * and compute shortest paths between locations.
 */
#include "Graph.h"
#include "NavigationMode.h"
#include <vector>
#include <memory>
#include <stdexcept>

/**
 * @class PathNotFoundException
 * @brief Custom exception for path not found errors
 * 
 * EXCEPTION HANDLING: Custom exception class
 */
class PathNotFoundException : public std::runtime_error {
public:
    explicit PathNotFoundException(const std::string& message)
        : std::runtime_error(message) {}
};

/**
 * @class InvalidLocationException
 * @brief Custom exception for invalid location errors
 * 
 * EXCEPTION HANDLING: Custom exception class
 */
class InvalidLocationException : public std::invalid_argument {
public:
    explicit InvalidLocationException(const std::string& message)
        : std::invalid_argument(message) {}
};

/**
 * @class Navigator
 * @brief Handles pathfinding and navigation
 * 
 * Demonstrates ABSTRACTION by:
 * - Hiding Dijkstra algorithm implementation
 * - Providing simple public interface
 * - Managing navigation modes
 * 
 * Demonstrates EXCEPTION HANDLING by:
 * - Throwing custom exceptions
 * - Validating inputs
 * - Handling error conditions
 */
class Navigator {
private:
    Graph<Location*> graph_;                    ///< Campus graph
    std::vector<Location*> allLocations_;       ///< All campus locations
    std::shared_ptr<NavigationMode> currentMode_; ///< Current navigation mode
    Path lastPath_;                             ///< Last calculated path
    
    /**
     * @class ViaSelectionException
     * @brief Exception thrown when an invalid via is selected
     */
    class ViaSelectionException : public std::runtime_error {
    public:
        explicit ViaSelectionException(const std::string& message)
            : std::runtime_error(message) {}
    };
    
    /**
     * @brief Dijkstra's algorithm implementation (PRIVATE - ABSTRACTION)
     * @param start Start location
     * @param end End location
     * @return Shortest path
     * @throws PathNotFoundException if no path exists
     * 
     * This method is private - users don't need to know how pathfinding works
     */
    Path dijkstraShortestPath(Location* start, Location* end);
    
    /**
     * @brief Reconstruct path from Dijkstra results
     * @param start Start location
     * @param end End location
     * @param previous Map of previous nodes
     * @param distances Map of distances
     * @return Reconstructed path
     */
    Path reconstructPath(Location* start, Location* end,
                         const std::map<Location*, Location*>& previous,
                         const std::map<Location*, double>& distances);
    
public:
    /**
     * @brief Constructor
     */
    Navigator();
    
    /**
     * @brief Destructor
     */
    ~Navigator();
    
    /**
     * @brief Initialize graph with locations and connections
     * @param locations Vector of all locations
     * @param connections Vector of path connections
     */
    void initializeGraph(const std::vector<Location*>& locations,
                         const std::vector<std::pair<int, int>>& connections,
                         const std::vector<double>& distances);
    
    /**
     * @brief Find shortest path between two locations (PUBLIC - SIMPLE INTERFACE)
     * @param startName Start location name
     * @param endName End location name
     * @return Shortest path
     * @throws InvalidLocationException if location not found
     * @throws PathNotFoundException if no path exists
     * 
     * ABSTRACTION: Simple interface hides complex implementation
     * EXCEPTION HANDLING: Throws exceptions for error cases
     */
    Path findPath(const std::string& startName, const std::string& endName);
    
    /**
     * @brief Find path by location pointers
     * @param start Start location
     * @param end End location
     * @return Shortest path
     */
    Path findPath(Location* start, Location* end);
    /**
     * @brief Find path that passes through given via locations in order
     * @param start Start location
     * @param end End location
     * @param vias Ordered vector of via locations (may be empty)
     * @return Combined path going through all vias
     * @throws ViaSelectionException if a via equals start or end
     */
    Path findPath(Location* start, Location* end, const std::vector<Location*>& vias);
    
    /**
     * @brief Set navigation mode
     * @param mode Navigation mode pointer
     */
    void setNavigationMode(std::shared_ptr<NavigationMode> mode);
    
    /**
     * @brief Get current navigation mode
     * @return Navigation mode pointer
     */
    std::shared_ptr<NavigationMode> getNavigationMode() const;
    
    /**
     * @brief Calculate estimated time for last path
     * @return Time in minutes
     */
    double getEstimatedTime() const;
    
    /**
     * @brief Get location by name
     * @param name Location name
     * @return Location pointer
     * @throws InvalidLocationException if not found
     */
    Location* getLocationByName(const std::string& name);
    
    /**
     * @brief Get all locations
     * @return Vector of all locations
     */
    std::vector<Location*> getAllLocations() const;
    
    /**
     * @brief Get the graph
     * @return Reference to the graph
     */
    const Graph<Location*>& getGraph() const;
    
    /**
     * @brief Get last calculated path
     * @return Last path
     */
    Path getLastPath() const;
};

#endif // NAVIGATOR_H
