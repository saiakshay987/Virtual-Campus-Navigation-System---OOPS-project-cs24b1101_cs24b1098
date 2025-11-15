/**
 * @file main.cpp
 * @brief Entry point for Virtual Campus Navigator
 * 
 * This application demonstrates all major OOP concepts:
 * - Encapsulation (Location class)
 * - Inheritance (AcademicBuilding, HostelBuilding)
 * - Polymorphism (NavigationMode hierarchy)
 * - Abstraction (Navigator class)
 * - Operator Overloading (Path class)
 * - Templates (Graph class)
 * - Exception Handling (Custom exceptions)
 */

#include <iostream>
#include <vector>
#include <memory>

#include "CampusData.h"
#include "Location.h"
#include "AcademicBuilding.h"
#include "HostelBuilding.h"
#include "Navigator.h"
#include "GUIHandler.h"
#include "WalkingMode.h"
#include "CyclingMode.h"

/**
 * @brief Initialize all campus locations
 * @return Vector of location pointers
 */
std::vector<Location*> initializeLocations() {
    std::vector<Location*> locations;
    
    // Create location objects from campus data
    for (size_t i = 0; i < CampusData::BUILDINGS.size(); ++i) {
        const auto& building = CampusData::BUILDINGS[i];
        
        Location* loc = nullptr;
        
        // Create appropriate derived class based on type
        if (building.buildingType == "Academic") {
            AcademicBuilding* academic = new AcademicBuilding(
                building.name,
                building.latitude,
                building.longitude,
                building.description,
                static_cast<int>(i)
            );
            
            // Set academic-specific properties
            if (building.name == "Academic Block") {
                academic->addDepartment("Computer Science");
                academic->addDepartment("Electronics");
                academic->addDepartment("Mechanical");
                academic->setNumberOfClassrooms(20);
                academic->setNumberOfLabs(10);
            } else if (building.name == "Lab Complex") {
                academic->addDepartment("Computer Science");
                academic->addDepartment("Electronics");
                academic->setNumberOfClassrooms(5);
                academic->setNumberOfLabs(15);
            }
            
            loc = academic;
        }
        else if (building.buildingType == "Hostel") {
            HostelBuilding* hostel = new HostelBuilding(
                building.name,
                building.latitude,
                building.longitude,
                building.description,
                static_cast<int>(i)
            );
            
            // Set hostel-specific properties
            hostel->setCapacity(550);
            hostel->setCurrentOccupancy(480);
            hostel->setNumberOfFloors(4);
            hostel->setHasCommonRoom(true);
            
            // Assign gender types (example)
            if (building.name == "Hostel A" || building.name == "Hostel B") {
                hostel->setGenderType(Gender::MALE);
            } else {
                hostel->setGenderType(Gender::FEMALE);
            }
            
            loc = hostel;
        }
        else {
            // Generic location for other buildings
            loc = new Location(
                building.name,
                building.latitude,
                building.longitude,
                building.description,
                static_cast<int>(i)
            );
        }
        
        locations.push_back(loc);
    }
    
    return locations;
}

/**
 * @brief Build connection and distance vectors from campus data
 */
void buildConnectionData(std::vector<std::pair<int, int>>& connections,
                        std::vector<double>& distances) {
    // Build index map for quick lookup
    std::map<std::string, int> nameToIndex;
    for (size_t i = 0; i < CampusData::BUILDINGS.size(); ++i) {
        nameToIndex[CampusData::BUILDINGS[i].name] = static_cast<int>(i);
    }
    
    // Convert path connections to index pairs
    for (const auto& path : CampusData::PATHS) {
        int fromIndex = nameToIndex[path.from];
        int toIndex = nameToIndex[path.to];
        
        connections.push_back({fromIndex, toIndex});
        distances.push_back(path.distanceMeters);
    }
}

/**
 * @brief Demonstrate OOP concepts (console output)
 */
void demonstrateOOPConcepts(const std::vector<Location*>& locations) {
    std::cout << "\\n========================================\\n";
    std::cout << "OOP CONCEPTS DEMONSTRATION\\n";
    std::cout << "========================================\\n\\n";
    
    // 1. ENCAPSULATION
    std::cout << "1. ENCAPSULATION:\\n";
    std::cout << "   Location class hides internal data\\n";
    Location* loc = locations;
    std::cout << "   Name: " << loc->getName() << "\\n";
    std::cout << "   Coordinates: (" << loc->getLatitude() 
              << ", " << loc->getLongitude() << ")\\n\\n";
    
    // 2. INHERITANCE & POLYMORPHISM
    std::cout << "2. INHERITANCE & POLYMORPHISM:\\n";
    for (Location* location : locations) {
        // Polymorphism: virtual method call
        location->displayInfo();
        std::cout << "\\n";
        
        // Only show first 3 for brevity
        if (location->getId() >= 2) break;
    }
    
    // 3. OPERATOR OVERLOADING
    std::cout << "3. OPERATOR OVERLOADING:\\n";
    Path path1(locations);
    path1.addLocation(locations);
    
    Path path2(locations);
    path2.addLocation(locations);
    
    std::cout << "   Path 1: ";
    path1.print();
    std::cout << "   Path 2: ";
    path2.print();
    
    Path combined = path1 + path2;  // Operator overloading
    std::cout << "   Combined (path1 + path2): ";
    combined.print();
    
    std::cout << "   path1 < path2? " << (path1 < path2 ? "Yes" : "No") << "\\n\\n";
    
    // 4. TEMPLATES
    std::cout << "4. TEMPLATES:\\n";
    std::cout << "   Graph<Location*> is a template class\\n";
    std::cout << "   Works with any node type\\n\\n";
    
    // 5. ABSTRACTION
    std::cout << "5. ABSTRACTION:\\n";
    std::cout << "   Navigator class hides Dijkstra implementation\\n";
    std::cout << "   Simple interface: findPath(start, end)\\n\\n";
    
    // 6. POLYMORPHISM (Navigation Modes)
    std::cout << "6. POLYMORPHISM (Navigation Modes):\\n";
    std::shared_ptr<NavigationMode> walking = std::make_shared<WalkingMode>();
    std::shared_ptr<NavigationMode> cycling = std::make_shared<CyclingMode>();
    
    double distance = 200.0; // 200 meters
    std::cout << "   Distance: " << distance << "m\\n";
    std::cout << "   Walking time: " << walking->calculateTime(distance) << " min\\n";
    std::cout << "   Cycling time: " << cycling->calculateTime(distance) << " min\\n\\n";
    
    // 7. EXCEPTION HANDLING
    std::cout << "7. EXCEPTION HANDLING:\\n";
    std::cout << "   Navigator throws custom exceptions\\n";
    std::cout << "   - InvalidLocationException\\n";
    std::cout << "   - PathNotFoundException\\n\\n";
}

/**
 * @brief Main function
 */
int main() {
    std::cout << "========================================\\n";
    std::cout << "Virtual Campus Navigator\\n";
    std::cout << "IIITDM Kancheepuram\\n";
    std::cout << "========================================\\n";
    std::cout << "\\nInitializing campus data...\\n";
    
    try {
        // Initialize locations
        std::vector<Location*> locations = initializeLocations();
        std::cout << "Loaded " << locations.size() << " campus buildings\\n";
        
        // Build connection data
        std::vector<std::pair<int, int>> connections;
        std::vector<double> distances;
        buildConnectionData(connections, distances);
        std::cout << "Loaded " << connections.size() << " path connections\\n";
        
        // Create navigator
        Navigator navigator;
        navigator.initializeGraph(locations, connections, distances);
        std::cout << "Graph initialized successfully\\n";
        
        // Demonstrate OOP concepts
        demonstrateOOPConcepts(locations);
        
        // Test pathfinding
        std::cout << "========================================\\n";
        std::cout << "PATHFINDING TEST\\n";
        std::cout << "========================================\\n\\n";
        
        std::string start = "Hostel A";
        std::string end = "Academic Block";
        
        std::cout << "Finding path from " << start << " to " << end << "\\n\\n";
        
        // Walking mode
        navigator.setNavigationMode(std::make_shared<WalkingMode>());
        Path walkPath = navigator.findPath(start, end);
        std::cout << "Walking Mode:\\n";
        walkPath.print();
        std::cout << "Distance: " << walkPath.getTotalDistance() << "m\\n";
        std::cout << "Time: " << navigator.getEstimatedTime() << " minutes\\n\\n";
        
        // Cycling mode
        navigator.setNavigationMode(std::make_shared<CyclingMode>());
        Path cyclePath = navigator.findPath(start, end);
        std::cout << "Cycling Mode:\\n";
        cyclePath.print();
        std::cout << "Distance: " << cyclePath.getTotalDistance() << "m\\n";
        std::cout << "Time: " << navigator.getEstimatedTime() << " minutes\\n\\n";
        
        // Launch GUI
        std::cout << "Launching GUI...\\n";
        GUIHandler gui(navigator);
        
        if (gui.initialize()) {
            gui.run();
        } else {
            std::cerr << "Failed to initialize GUI\\n";
            return 1;
        }
        
        // Cleanup
        for (Location* loc : locations) {
            delete loc;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
