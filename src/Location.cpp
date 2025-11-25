/**
 * @file Location.cpp
 * @brief Implementation of Location class
 */

#include "Location.h"

/**
 * @file Location.cpp
 * @brief Implementation for the Location base class.
 *
 * Provides basic location properties and distance helpers.
 */
#include <iostream>
#include <stdexcept>
#include <cmath>

// Mathematical constants
const double PI = 3.14159265358979323846;
const double EARTH_RADIUS_METERS = 6371000.0; // Earth's radius in meters

// Default constructor
Location::Location() 
    : latitude_(0.0), longitude_(0.0), name_("Unknown"), 
      description_(""), id_(-1) {
}

// Parameterized constructor
Location::Location(const std::string& name, double lat, double lon, 
                   const std::string& desc, int id)
    : name_(name), description_(desc), id_(id) {
    // Use setters for validation
    setLatitude(lat);
    setLongitude(lon);
}

// Virtual destructor
Location::~Location() {
    // Clean up if needed
}

// Getter implementations
std::string Location::getName() const {
    return name_;
}

double Location::getLatitude() const {
    return latitude_;
}

double Location::getLongitude() const {
    return longitude_;
}

std::string Location::getDescription() const {
    return description_;
}

int Location::getId() const {
    return id_;
}

// Setter implementations with validation
void Location::setName(const std::string& name) {
    if (name.empty()) {
        throw std::invalid_argument("Location name cannot be empty");
    }
    name_ = name;
}

void Location::setLatitude(double lat) {
    if (lat < -90.0 || lat > 90.0) {
        throw std::invalid_argument("Latitude must be between -90 and 90 degrees");
    }
    latitude_ = lat;
}

void Location::setLongitude(double lon) {
    if (lon < -180.0 || lon > 180.0) {
        throw std::invalid_argument("Longitude must be between -180 and 180 degrees");
    }
    longitude_ = lon;
}

void Location::setDescription(const std::string& desc) {
    description_ = desc;
}

void Location::setId(int id) {
    id_ = id;
}

/**
 * @brief Calculate distance using Haversine formula
 * 
 * The Haversine formula calculates the great-circle distance between two points
 * on a sphere given their longitudes and latitudes.
 * 
 * Formula:
 * a = sin²(Δφ/2) + cos(φ1) × cos(φ2) × sin²(Δλ/2)
 * c = 2 × atan2(√a, √(1−a))
 * d = R × c
 * 
 * where φ is latitude, λ is longitude, R is earth's radius
 */
double Location::distanceTo(const Location& other) const {
    // Convert degrees to radians
    double lat1Rad = latitude_ * PI / 180.0;
    double lat2Rad = other.latitude_ * PI / 180.0;
    double deltaLat = (other.latitude_ - latitude_) * PI / 180.0;
    double deltaLon = (other.longitude_ - longitude_) * PI / 180.0;
    
    // Haversine formula
    double a = std::sin(deltaLat / 2.0) * std::sin(deltaLat / 2.0) +
               std::cos(lat1Rad) * std::cos(lat2Rad) *
               std::sin(deltaLon / 2.0) * std::sin(deltaLon / 2.0);
    
    double c = 2.0 * std::atan2(std::sqrt(a), std::sqrt(1.0 - a));
    
    double distance = EARTH_RADIUS_METERS * c;
    
    return distance; // Distance in meters
}

// Virtual method for polymorphism
void Location::displayInfo() const {
    std::cout << "Location: " << name_ << std::endl;
    std::cout << "  Coordinates: (" << latitude_ << ", " << longitude_ << ")" << std::endl;
    std::cout << "  Description: " << description_ << std::endl;
    std::cout << "  ID: " << id_ << std::endl;
}

// Private validation method
bool Location::validateCoordinates(double lat, double lon) const {
    return (lat >= -90.0 && lat <= 90.0 && lon >= -180.0 && lon <= 180.0);
}
