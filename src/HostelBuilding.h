/**
 * @file HostelBuilding.h
 * @brief Another derived class demonstrating INHERITANCE
 * 
 * OOP Concept: Inheritance
 * - Inherits from Location base class
 * - Adds hostel-specific properties
 * - Demonstrates polymorphism through method overriding
 */

#ifndef HOSTEL_BUILDING_H
#define HOSTEL_BUILDING_H

/**
 * @file HostelBuilding.h
 * @brief HostelBuilding derived class declaration.
 *
 * Extends Location with hostel-specific properties (capacity, gender).
 */

#include "Location.h"

/**
 * @enum Gender
 * @brief Gender type for hostel
 */
enum class Gender {
    MALE,
    FEMALE,
    COED
};

/**
 * @class HostelBuilding
 * @brief Represents a hostel building (inherits from Location)
 * 
 * Demonstrates INHERITANCE by:
 * - Extending Location base class
 * - Adding hostel-specific properties
 * - Overriding displayInfo() method
 */
class HostelBuilding : public Location {
private:
    int capacity_;          ///< Maximum number of students
    int currentOccupancy_;  ///< Current number of students
    Gender genderType_;     ///< Gender type of hostel
    int numberOfFloors_;    ///< Number of floors
    bool hasCommonRoom_;    ///< Has common room facility

public:
    /**
     * @brief Constructor
     * @param name Building name
     * @param lat Latitude
     * @param lon Longitude
     * @param desc Description
     * @param id Unique ID
     */
    HostelBuilding(const std::string& name, double lat, double lon,
                   const std::string& desc, int id);
    
    /**
     * @brief Destructor
     */
    virtual ~HostelBuilding();
    
    /**
     * @brief Set hostel capacity
     * @param cap Maximum capacity
     */
    void setCapacity(int cap);
    
    /**
     * @brief Get hostel capacity
     * @return Maximum capacity
     */
    int getCapacity() const;
    
    /**
     * @brief Set current occupancy
     * @param occ Current number of students
     */
    void setCurrentOccupancy(int occ);
    
    /**
     * @brief Get current occupancy
     * @return Current number of students
     */
    int getCurrentOccupancy() const;
    
    /**
     * @brief Get availability
     * @return Number of available beds
     */
    int getAvailability() const;
    
    /**
     * @brief Set gender type
     * @param gender Gender type
     */
    void setGenderType(Gender gender);
    
    /**
     * @brief Get gender type
     * @return Gender type
     */
    Gender getGenderType() const;
    
    /**
     * @brief Set number of floors
     * @param floors Number of floors
     */
    void setNumberOfFloors(int floors);
    
    /**
     * @brief Get number of floors
     * @return Number of floors
     */
    int getNumberOfFloors() const;
    
    /**
     * @brief Set common room availability
     * @param hasRoom True if has common room
     */
    void setHasCommonRoom(bool hasRoom);
    
    /**
     * @brief Check common room availability
     * @return True if has common room
     */
    bool hasCommonRoom() const;
    
    /**
     * @brief Override displayInfo to show hostel-specific information
     * POLYMORPHISM: Override base class method
     */
    virtual void displayInfo() const override;
};

#endif // HOSTEL_BUILDING_H
