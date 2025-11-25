/**
 * @file HostelBuilding.cpp
 * @brief Implementation of HostelBuilding class
 */

#include "HostelBuilding.h"

/**
 * @file HostelBuilding.cpp
 * @brief Implementation for HostelBuilding methods.
 */
#include <iostream>

/**
 * @brief Constructor
 */
HostelBuilding::HostelBuilding(const std::string& name, double lat, double lon,
                               const std::string& desc, int id)
    : Location(name, lat, lon, desc, id),
      capacity_(0),
      currentOccupancy_(0),
      genderType_(Gender::COED),
      numberOfFloors_(0),
      hasCommonRoom_(false) {
}

/**
 * @brief Destructor
 */
HostelBuilding::~HostelBuilding() {
}

/**
 * @brief Set hostel capacity
 */
void HostelBuilding::setCapacity(int cap) {
    capacity_ = cap;
}

/**
 * @brief Get hostel capacity
 */
int HostelBuilding::getCapacity() const {
    return capacity_;
}

/**
 * @brief Set current occupancy
 */
void HostelBuilding::setCurrentOccupancy(int occ) {
    currentOccupancy_ = occ;
}

/**
 * @brief Get current occupancy
 */
int HostelBuilding::getCurrentOccupancy() const {
    return currentOccupancy_;
}

/**
 * @brief Get availability
 */
int HostelBuilding::getAvailability() const {
    return capacity_ - currentOccupancy_;
}

/**
 * @brief Set gender type
 */
void HostelBuilding::setGenderType(Gender gender) {
    genderType_ = gender;
}

/**
 * @brief Get gender type
 */
Gender HostelBuilding::getGenderType() const {
    return genderType_;
}

/**
 * @brief Set number of floors
 */
void HostelBuilding::setNumberOfFloors(int floors) {
    numberOfFloors_ = floors;
}

/**
 * @brief Get number of floors
 */
int HostelBuilding::getNumberOfFloors() const {
    return numberOfFloors_;
}

/**
 * @brief Set common room availability
 */
void HostelBuilding::setHasCommonRoom(bool hasRoom) {
    hasCommonRoom_ = hasRoom;
}

/**
 * @brief Check if hostel has common room
 */
bool HostelBuilding::hasCommonRoom() const {
    return hasCommonRoom_;
}

/**
 * @brief Override displayInfo to show hostel-specific information
 */
void HostelBuilding::displayInfo() const {
    std::cout << "Hostel Building: " << getName() << std::endl;
    std::cout << "  Location: (" << getLatitude() << ", " << getLongitude() << ")" << std::endl;
    std::cout << "  Description: " << getDescription() << std::endl;
    std::cout << "  Capacity: " << capacity_ << std::endl;
    std::cout << "  Current Occupancy: " << currentOccupancy_ << std::endl;
    std::cout << "  Available Beds: " << getAvailability() << std::endl;
    std::cout << "  Number of Floors: " << numberOfFloors_ << std::endl;
    std::cout << "  Has Common Room: " << (hasCommonRoom_ ? "Yes" : "No") << std::endl;
    std::string gender_str;
    switch (genderType_) {
        case Gender::MALE:
            gender_str = "Male";
            break;
        case Gender::FEMALE:
            gender_str = "Female";
            break;
        case Gender::COED:
            gender_str = "Coed";
            break;
    }
    std::cout << "  Gender Type: " << gender_str << std::endl;
}
