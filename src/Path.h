/**
 * @file Path.h
 * @brief Path class demonstrating OPERATOR OVERLOADING
 * 
 * OOP Concept: Operator Overloading
 * - Overloads + operator to combine paths
 * - Overloads == operator to compare paths
 * - Overloads < operator for path comparison
 * - Makes path operations more intuitive
 */

#ifndef PATH_H
#define PATH_H

/**
 * @file Path.h
 * @brief Path container declaration used to represent a sequence of Locations.
 *
 * Provides storage and utilities for route composition and distance accounting.
 */

#include "Location.h"
#include <vector>
#include <memory>

/**
 * @class Path
 * @brief Represents a path through multiple locations
 * 
 * Demonstrates OPERATOR OVERLOADING by:
 * - Overloading arithmetic operators (+)
 * - Overloading comparison operators (==, <)
 * - Making path operations intuitive
 */
class Path {
private:
    std::vector<Location*> locations_;  ///< Sequence of locations in path
    double totalDistance_;               ///< Total path distance in meters
    
public:
    /**
     * @brief Default constructor
     */
    Path();
    
    /**
     * @brief Constructor with starting location
     * @param start Starting location pointer
     */
    explicit Path(Location* start);
    
    /**
     * @brief Add a location to the path
     * @param loc Location pointer to add
     */
    void addLocation(Location* loc);
    
    /**
     * @brief Get all locations in path
     * @return Vector of location pointers
     */
    std::vector<Location*> getLocations() const;
    
    /**
     * @brief Get total distance
     * @return Total distance in meters
     */
    double getTotalDistance() const;
    
    /**
     * @brief Set total distance manually
     * @param dist Distance in meters
     */
    void setTotalDistance(double dist);
    
    /**
     * @brief Calculate total distance from locations
     */
    void calculateTotalDistance();
    
    /**
     * @brief Get number of locations in path
     * @return Number of locations
     */
    size_t size() const;
    
    /**
     * @brief Check if path is empty
     * @return True if empty
     */
    bool empty() const;
    
    /**
     * @brief Clear the path
     */
    void clear();
    
    // OPERATOR OVERLOADING
    
    /**
     * @brief Overload + operator to combine two paths
     * @param other Another path
     * @return Combined path
     * 
     * Example: Path p3 = p1 + p2;
     */
    Path operator+(const Path& other) const;
    
    /**
     * @brief Overload == operator to compare two paths
     * @param other Another path
     * @return True if paths are equal
     * 
     * Example: if (p1 == p2) { ... }
     */
    bool operator==(const Path& other) const;
    
    /**
     * @brief Overload != operator
     * @param other Another path
     * @return True if paths are different
     */
    bool operator!=(const Path& other) const;
    
    /**
     * @brief Overload < operator to compare path lengths
     * @param other Another path
     * @return True if this path is shorter
     * 
     * Example: if (p1 < p2) { ... }
     * Useful for sorting paths by distance
     */
    bool operator<(const Path& other) const;
    
    /**
     * @brief Overload > operator
     * @param other Another path
     * @return True if this path is longer
     */
    bool operator>(const Path& other) const;
    
    /**
     * @brief Overload [] operator to access location by index
     * @param index Index of location
     * @return Location pointer at index
     */
    Location* operator[](size_t index) const;
    
    /**
     * @brief Print path information
     */
    void print() const;
};

#endif // PATH_H
