/**
 * @file CyclingMode.h
 * @brief Cycling navigation mode (demonstrates POLYMORPHISM)
 * 
 * OOP Concept: Polymorphism
 * - Derives from NavigationMode abstract class
 * - Implements pure virtual methods
 * - Provides cycling-specific behavior
 */

#ifndef CYCLING_MODE_H
#define CYCLING_MODE_H

#include "NavigationMode.h"

/**
 * @class CyclingMode
 * @brief Navigation mode for cycling
 * 
 * Demonstrates POLYMORPHISM by:
 * - Inheriting from abstract NavigationMode class
 * - Overriding pure virtual methods
 * - Providing cycling-specific implementation
 */
class CyclingMode : public NavigationMode {
public:
    /**
     * @brief Constructor
     * Average cycling speed: 15 km/h (typical campus cycling pace)
     */
    CyclingMode()
        : NavigationMode("Cycling", 15.0) {
    /**
     * @file CyclingMode.h
     * @brief Cycling navigation mode implementation declaration.
     *
     * Calculates time estimates for cycling between points.
     */
    }
    
    /**
     * @brief Destructor
     */
    virtual ~CyclingMode() {}
    
    /**
     * @brief Calculate cycling time
     * @param distanceMeters Distance in meters
     * @return Time in minutes
     * 
     * POLYMORPHISM: Override pure virtual method
     * Formula: time = distance / speed
     * Speed: 15 km/h = 250 m/min
     */
    virtual double calculateTime(double distanceMeters) const override {
        // Convert speed from km/h to m/min
        double speedMetersPerMin = averageSpeed_ * 1000.0 / 60.0;
        
        // Calculate time in minutes
        double timeMinutes = distanceMeters / speedMetersPerMin;
        
        return timeMinutes;
    }
    
    /**
     * @brief Get cycling mode description
     * @return Description string
     * 
     * POLYMORPHISM: Override pure virtual method
     */
    virtual std::string getDescription() const override {
        return "Cycling mode: Average pace of 15 km/h. "
               "Faster travel for longer campus routes. ";
    }
    
    /**
     * @brief Get cycling icon
     * @return Cycling icon character
     * 
     * POLYMORPHISM: Override virtual method
     */
    virtual char getIcon() const override {
        return 'C';
    }
};

#endif // CYCLING_MODE_H
