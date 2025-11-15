/**
 * @file Path.cpp
 * @brief Implementation of Path class with operator overloading
 */

#include "Path.h"
#include <iostream>
#include <stdexcept>

// Default constructor
Path::Path() : totalDistance_(0.0) {
}

// Constructor with starting location
Path::Path(Location* start) : totalDistance_(0.0) {
    if (start != nullptr) {
        locations_.push_back(start);
    }
}

// Add location to path
void Path::addLocation(Location* loc) {
    if (loc == nullptr) {
        throw std::invalid_argument("Cannot add null location to path");
    }
    
    // If not first location, add distance from last location
    if (!locations_.empty()) {
        Location* lastLoc = locations_.back();
        totalDistance_ += lastLoc->distanceTo(*loc);
    }
    
    locations_.push_back(loc);
}

// Get all locations
std::vector<Location*> Path::getLocations() const {
    return locations_;
}

// Get total distance
double Path::getTotalDistance() const {
    return totalDistance_;
}

// Set total distance
void Path::setTotalDistance(double dist) {
    if (dist < 0) {
        throw std::invalid_argument("Distance cannot be negative");
    }
    totalDistance_ = dist;
}

// Calculate total distance
void Path::calculateTotalDistance() {
    totalDistance_ = 0.0;
    for (size_t i = 1; i < locations_.size(); ++i) {
        totalDistance_ += locations_[i-1]->distanceTo(*locations_[i]);
    }
}

// Get size
size_t Path::size() const {
    return locations_.size();
}

// Check if empty
bool Path::empty() const {
    return locations_.empty();
}

// Clear path
void Path::clear() {
    locations_.clear();
    totalDistance_ = 0.0;
}

// OPERATOR OVERLOADING IMPLEMENTATIONS

/**
 * @brief Overload + operator to combine paths
 * 
 * Combines two paths into one continuous path.
 * If last location of first path matches first location of second path,
 * it avoids duplication.
 */
Path Path::operator+(const Path& other) const {
    Path combined;
    
    // Add all locations from this path
    for (Location* loc : locations_) {
        if (combined.empty()) {
            combined.locations_.push_back(loc);
        } else {
            combined.addLocation(loc);
        }
    }
    
    // Add locations from other path
    size_t startIndex = 0;
    if (!combined.empty() && !other.empty()) {
        // Skip first location of other path if it matches last of this path
        if (combined.locations_.back()->getId() == other.locations_->getId()) {
            startIndex = 1;
        }
    }
    
    for (size_t i = startIndex; i < other.locations_.size(); ++i) {
        combined.addLocation(other.locations_[i]);
    }
    
    return combined;
}

/**
 * @brief Overload == operator
 * 
 * Two paths are equal if they contain the same sequence of locations
 */
bool Path::operator==(const Path& other) const {
    if (locations_.size() != other.locations_.size()) {
        return false;
    }
    
    for (size_t i = 0; i < locations_.size(); ++i) {
        if (locations_[i]->getId() != other.locations_[i]->getId()) {
            return false;
        }
    }
    
    return true;
}

/**
 * @brief Overload != operator
 */
bool Path::operator!=(const Path& other) const {
    return !(*this == other);
}

/**
 * @brief Overload < operator for distance comparison
 */
bool Path::operator<(const Path& other) const {
    return totalDistance_ < other.totalDistance_;
}

/**
 * @brief Overload > operator
 */
bool Path::operator>(const Path& other) const {
    return totalDistance_ > other.totalDistance_;
}

/**
 * @brief Overload [] operator for array-like access
 */
Location* Path::operator[](size_t index) const {
    if (index >= locations_.size()) {
        throw std::out_of_range("Path index out of range");
    }
    return locations_[index];
}

// Print path
void Path::print() const {
    std::cout << "Path (" << totalDistance_ << "m): ";
    for (size_t i = 0; i < locations_.size(); ++i) {
        std::cout << locations_[i]->getName();
        if (i < locations_.size() - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << std::endl;
}
