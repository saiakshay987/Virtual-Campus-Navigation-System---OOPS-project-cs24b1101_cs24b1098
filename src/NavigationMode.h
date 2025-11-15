/**
 * @file NavigationMode.h
 * @brief Abstract base class demonstrating POLYMORPHISM
 * 
 * OOP Concept: Polymorphism (Runtime Polymorphism)
 * - Abstract base class with pure virtual methods
 * - Derived classes implement specific behavior
 * - Enables flexible navigation mode selection
 */

#ifndef NAVIGATION_MODE_H
#define NAVIGATION_MODE_H

#include <string>

/**
 * @class NavigationMode
 * @brief Abstract base class for different navigation modes
 * 
 * Demonstrates POLYMORPHISM by:
 * - Defining pure virtual methods
 * - Allowing derived classes to override behavior
 * - Enabling runtime polymorphism
 * 
 * This is an abstract class that cannot be instantiated directly.
 * Derived classes must implement all pure virtual methods.
 */
class NavigationMode {
protected:
    std::string modeName_;      ///< Name of the navigation mode
    double averageSpeed_;       ///< Average speed in km/h
    
public:
    /**
     * @brief Constructor
     * @param name Mode name
     * @param speed Average speed in km/h
     */
    NavigationMode(const std::string& name, double speed)
        : modeName_(name), averageSpeed_(speed) {}
    
    /**
     * @brief Virtual destructor
     */
    virtual ~NavigationMode() {}
    
    /**
     * @brief Get mode name
     * @return Mode name
     */
    std::string getModeName() const {
        return modeName_;
    }
    
    /**
     * @brief Get average speed
     * @return Speed in km/h
     */
    double getAverageSpeed() const {
        return averageSpeed_;
    }
    
    /**
     * @brief Calculate time to travel given distance (PURE VIRTUAL)
     * @param distanceMeters Distance in meters
     * @return Time in minutes
     * 
     * POLYMORPHISM: Pure virtual method - must be overridden
     * This enables different calculation logic for each mode
     */
    virtual double calculateTime(double distanceMeters) const = 0;
    
    /**
     * @brief Get mode description (PURE VIRTUAL)
     * @return Description string
     * 
     * POLYMORPHISM: Pure virtual method
     */
    virtual std::string getDescription() const = 0;
    
    /**
     * @brief Get mode icon/symbol (VIRTUAL - can be overridden)
     * @return Icon character
     */
    virtual char getIcon() const {
        return '?';
    }
};

#endif // NAVIGATION_MODE_H
