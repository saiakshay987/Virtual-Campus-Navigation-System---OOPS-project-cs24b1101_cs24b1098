/**
 * @file AcademicBuilding.cpp
 * @brief Implementation of AcademicBuilding class
 */

#include "AcademicBuilding.h"

/**
 * @file AcademicBuilding.cpp
 * @brief Implementation of AcademicBuilding methods.
 */
#include <iostream>

/**
 * @brief Constructor
 */
AcademicBuilding::AcademicBuilding(const std::string& name, double lat, double lon,
                                   const std::string& desc, int id)
    : Location(name, lat, lon, desc, id),
      numberOfClassrooms_(0),
      numberOfLabs_(0),
      hasLibrary_(false) {
}

/**
 * @brief Destructor
 */
AcademicBuilding::~AcademicBuilding() {
}

/**
 * @brief Add a department to this building
 */
void AcademicBuilding::addDepartment(const std::string& deptName) {
    departments_.push_back(deptName);
}

/**
 * @brief Get list of departments
 */
std::vector<std::string> AcademicBuilding::getDepartments() const {
    return departments_;
}

/**
 * @brief Set number of classrooms
 */
void AcademicBuilding::setNumberOfClassrooms(int count) {
    numberOfClassrooms_ = count;
}

/**
 * @brief Get number of classrooms
 */
int AcademicBuilding::getNumberOfClassrooms() const {
    return numberOfClassrooms_;
}

/**
 * @brief Set number of labs
 */
void AcademicBuilding::setNumberOfLabs(int count) {
    numberOfLabs_ = count;
}

/**
 * @brief Get number of labs
 */
int AcademicBuilding::getNumberOfLabs() const {
    return numberOfLabs_;
}

/**
 * @brief Set library availability
 */
void AcademicBuilding::setHasLibrary(bool hasLib) {
    hasLibrary_ = hasLib;
}

/**
 * @brief Check if building has library
 */
bool AcademicBuilding::hasLibrary() const {
    return hasLibrary_;
}

/**
 * @brief Override displayInfo to show academic-specific information
 */
void AcademicBuilding::displayInfo() const {
    std::cout << "Academic Building: " << getName() << std::endl;
    std::cout << "  Location: (" << getLatitude() << ", " << getLongitude() << ")" << std::endl;
    std::cout << "  Description: " << getDescription() << std::endl;
    std::cout << "  Classrooms: " << numberOfClassrooms_ << std::endl;
    std::cout << "  Labs: " << numberOfLabs_ << std::endl;
    std::cout << "  Has Library: " << (hasLibrary_ ? "Yes" : "No") << std::endl;
    std::cout << "  Departments: ";
    for (const auto& dept : departments_) {
        std::cout << dept << " ";
    }
    std::cout << std::endl;
}
