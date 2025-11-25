/**
 * @file Location.h
 * @brief Location class demonstrating ENCAPSULATION
 * 
 * OOP Concept: Encapsulation
 * - Private data members (latitude, longitude, name, id)
 * - Public interface methods (getters/setters)
 * - Data validation in setters
 * - Hides internal representation from users
 */

#ifndef LOCATION_H
#define LOCATION_H

/**
 * @file Location.h
 * @brief Location base class declaration used to represent campus points.
 *
 * Declares the Location interface and common data used by building types.
 */

#include <string>
#include <cmath>

/**
 * @class Location
 * @brief Represents a geographical location on campus
 * 
 * Demonstrates ENCAPSULATION by:
 * - Keeping data members private
 * - Providing controlled access through public methods
 * - Validating inputs in setters
 */
class Location {
private:
    // ENCAPSULATION: Private data members
    double latitude_;       ///< GPS latitude coordinate
    double longitude_;      ///< GPS longitude coordinate
    std::string name_;      ///< Name of the location
    std::string description_; ///< Description of the location
    int id_;                ///< Unique identifier

public:
    /**
     * @brief Default constructor
     */
    Location();
    
    /**
     * @brief Parameterized constructor
     * @param name Location name
     * @param lat Latitude
     * @param lon Longitude
     * @param desc Description
     * @param id Unique ID
     */
    Location(const std::string& name, double lat, double lon, 
             const std::string& desc, int id);
    
    /**
     * @brief Virtual destructor for proper inheritance
     */
    virtual ~Location();
    
    // ENCAPSULATION: Public getter methods (read-only access)
    
    /**
     * @brief Get location name
     * @return Name string
     */
    std::string getName() const;
    
    /**
     * @brief Get latitude coordinate
     * @return Latitude value
     */
    double getLatitude() const;
    
    /**
     * @brief Get longitude coordinate
     * @return Longitude value
     */
    double getLongitude() const;
    
    /**
     * @brief Get location description
     * @return Description string
     */
    std::string getDescription() const;
    
    /**
     * @brief Get location ID
     * @return ID value
     */
    int getId() const;
    
    // ENCAPSULATION: Public setter methods (controlled write access with validation)
    
    /**
     * @brief Set location name
     * @param name New name
     */
    void setName(const std::string& name);
    
    /**
     * @brief Set latitude with validation
     * @param lat New latitude (-90 to 90)
     * @throws std::invalid_argument if latitude out of range
     */
    void setLatitude(double lat);
    
    /**
     * @brief Set longitude with validation
     * @param lon New longitude (-180 to 180)
     * @throws std::invalid_argument if longitude out of range
     */
    void setLongitude(double lon);
    
    /**
     * @brief Set location description
     * @param desc New description
     */
    void setDescription(const std::string& desc);
    
    /**
     * @brief Set location ID
     * @param id New ID
     */
    void setId(int id);
    
    /**
     * @brief Calculate distance to another location using Haversine formula
     * @param other Another location
     * @return Distance in meters
     */
    double distanceTo(const Location& other) const;
    
    /**
     * @brief Display location information (for polymorphism demonstration)
     */
    virtual void displayInfo() const;
    
private:
    /**
     * @brief Validate GPS coordinates (ENCAPSULATION: private helper method)
     * @param lat Latitude to validate
     * @param lon Longitude to validate
     * @return true if valid, false otherwise
     */
    bool validateCoordinates(double lat, double lon) const;
};

#endif // LOCATION_H
