/**
 * @file WalkingMode.h
 * @brief Walking navigation mode (demonstrates POLYMORPHISM)
 * 
 * OOP Concept: Polymorphism
 * - Derives from NavigationMode abstract class
 * - Implements pure virtual methods
 * - Provides walking-specific behavior
 */

#ifndef WALKING_MODE_H
#define WALKING_MODE_H

#include "NavigationMode.h"

/**
 * @class WalkingMode
 * @brief Navigation mode for walking
 * 
 * Demonstrates POLYMORPHISM by:
 * - Inheriting from abstract NavigationMode class
 * - Overriding pure virtual methods
 * - Providing walking-specific implementation
 */
class WalkingMode : public NavigationMode {
public:
    /**
     * @brief Constructor
     * Average walking speed: 5 km/h (typical campus walking pace)
     */
    WalkingMode() 
        : NavigationMode("Walking", 5.0) {
    }
    
    /**
     * @brief Destructor
     */
    virtual ~WalkingMode() {}
    
    /**
     * @brief Calculate walking time
     * @param distanceMeters Distance in meters
     * @return Time in minutes
     * 
     * POLYMORPHISM: Override pure virtual method
     * Formula: time = distance / speed
     * Speed: 5 km/h = 83.33 m/min
     */
    virtual double calculateTime(double distanceMeters) const override {
        // Convert speed from km/h to m/min
        double speedMetersPerMin = averageSpeed_ * 1000.0 / 60.0;
        
        // Calculate time in minutes
        double timeMinutes = distanceMeters / speedMetersPerMin;
        
        return timeMinutes;
    }
    
    /**
     * @brief Get walking mode description
     * @return Description string
     * 
     * POLYMORPHISM: Override pure virtual method
     */
    virtual std::string getDescription() const override {
        return "Walking mode: Average pace of 5 km/h. "
               "Suitable for all campus routes and pathways.";
    }
    
    /**
     * @brief Get walking icon
     * @return Walking icon character
     * 
     * POLYMORPHISM: Override virtual method
     */
    virtual char getIcon() const override {
        return 'W';
    }
};

#endif // WALKING_MODE_H
