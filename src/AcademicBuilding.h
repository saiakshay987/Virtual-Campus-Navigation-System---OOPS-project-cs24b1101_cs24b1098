/**
 * @file AcademicBuilding.h
 * @brief Derived class demonstrating INHERITANCE
 * 
 * OOP Concept: Inheritance
 * - Inherits from Location base class
 * - Adds specialized properties for academic buildings
 * - Overrides virtual methods from base class
 */

#ifndef ACADEMIC_BUILDING_H
#define ACADEMIC_BUILDING_H

#include "Location.h"
#include <vector>

/**
 * @class AcademicBuilding
 * @brief Represents an academic building (inherits from Location)
 * 
 * Demonstrates INHERITANCE by:
 * - Extending Location base class
 * - Adding academic-specific properties
 * - Overriding displayInfo() method
 */
class AcademicBuilding : public Location {
private:
    std::vector<std::string> departments_; ///< List of departments
    int numberOfClassrooms_;                ///< Number of classrooms
    int numberOfLabs_;                      ///< Number of laboratories
    bool hasLibrary_;                       ///< Whether building has library

public:
    /**
     * @brief Constructor
     * @param name Building name
     * @param lat Latitude
     * @param lon Longitude
     * @param desc Description
     * @param id Unique ID
     */
    AcademicBuilding(const std::string& name, double lat, double lon,
                     const std::string& desc, int id);
    
    /**
     * @brief Destructor
     */
    virtual ~AcademicBuilding();
    
    /**
     * @brief Add a department to this building
     * @param deptName Department name
     */
    void addDepartment(const std::string& deptName);
    
    /**
     * @brief Get list of departments
     * @return Vector of department names
     */
    std::vector<std::string> getDepartments() const;
    
    /**
     * @brief Set number of classrooms
     * @param count Number of classrooms
     */
    void setNumberOfClassrooms(int count);
    
    /**
     * @brief Get number of classrooms
     * @return Classroom count
     */
    int getNumberOfClassrooms() const;
    
    /**
     * @brief Set number of labs
     * @param count Number of labs
     */
    void setNumberOfLabs(int count);
    
    /**
     * @brief Get number of labs
     * @return Lab count
     */
    int getNumberOfLabs() const;
    
    /**
     * @brief Set library availability
     * @param hasLib True if building has library
     */
    void setHasLibrary(bool hasLib);
    
    /**
     * @brief Check if building has library
     * @return True if has library
     */
    bool hasLibrary() const;
    
    /**
     * @brief Override displayInfo to show academic-specific information
     * POLYMORPHISM: Override base class method
     */
    virtual void displayInfo() const override;
};

#endif // ACADEMIC_BUILDING_H
