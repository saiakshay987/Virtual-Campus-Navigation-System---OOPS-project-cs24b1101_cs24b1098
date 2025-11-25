/**
 * @file main.cpp
 * @brief Application entry point for the Virtual Campus Navigator demo.
 *
 * Initializes campus data, demonstrates OOP features, runs pathfinding
 * tests and launches the SFML GUI.
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

    // Append turn/waypoint nodes provided by the user (hidden labels)
    // These are not in CampusData::BUILDINGS; they are added at runtime.
    const std::vector<std::tuple<std::string, double, double>> turns = {
        {"turn_01", 12.840104, 80.1366685},
        {"turn_02", 12.839675, 80.136476},
        {"turn_03", 12.839026, 80.136186},
        {"turn_04", 12.838454, 80.135948},
        {"turn_05", 12.837093, 80.135299},
        {"turn_06", 12.837072, 80.136278},
        {"turn_07", 12.836302, 80.136296},
        {"turn_08", 12.835422, 80.137477},
        {"turn_09", 12.838457, 80.139066}
    };

    for (size_t t = 0; t < turns.size(); ++t) {
        const auto &tp = turns[t];
        std::string name = std::get<0>(tp);
        double lat = std::get<1>(tp);
        double lon = std::get<2>(tp);

        Location* turnLoc = new Location(name, lat, lon, std::string("[hidden]"), static_cast<int>(CampusData::BUILDINGS.size() + t));
        locations.push_back(turnLoc);
    }

    return locations;
}

/**
 * @brief Build connection and distance vectors from campus data
 */
void buildConnectionData(const std::vector<Location*>& locations, std::vector<std::pair<int, int>>& connections,
                        std::vector<double>& distances) {
    // Build index map for quick lookup from the runtime locations vector
    std::map<std::string, int> nameToIndex;
    for (size_t i = 0; i < locations.size(); ++i) {
        nameToIndex[locations[i]->getName()] = static_cast<int>(i);
    }
    // Helper: normalize strings for loose matching (lowercase, remove non-alnum)
    auto normalize = [](const std::string& s) {
        std::string out;
        for (char c : s) {
            if (std::isalnum((unsigned char)c)) out.push_back(std::tolower((unsigned char)c));
        }
        return out;
    };

    // Build a normalized name -> index map for fuzzy matching
    std::map<std::string, int> normToIndex;
    for (size_t i = 0; i < locations.size(); ++i) {
        normToIndex[normalize(locations[i]->getName())] = static_cast<int>(i);
    }

    // Haversine helper
    auto haversine = [](double lat1, double lon1, double lat2, double lon2) {
        const double R = 6371000.0; // meters
        const double toRad = 3.14159265358979323846 / 180.0;
        double dLat = (lat2 - lat1) * toRad;
        double dLon = (lon2 - lon1) * toRad;
        double a = std::sin(dLat/2) * std::sin(dLat/2) +
                   std::cos(lat1 * toRad) * std::cos(lat2 * toRad) *
                   std::sin(dLon/2) * std::sin(dLon/2);
        double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));
        return R * c;
    };

    // Convert path connections to index pairs based on CampusData::PATHS
    for (const auto& path : CampusData::PATHS) {
        std::string nfrom = normalize(path.from);
        std::string nto = normalize(path.to);

        // try direct normalized match
        auto itFrom = normToIndex.find(nfrom);
        auto itTo = normToIndex.find(nto);

        if (itFrom == normToIndex.end() || itTo == normToIndex.end()) {
            // not found: skip this path (could log)
            continue;
        }

        int fromIndex = itFrom->second;
        int toIndex = itTo->second;

        double dist = path.distanceMeters;
        if (dist <= 0.0) {
            // compute from runtime locations
            double lat1 = locations[fromIndex]->getLatitude();
            double lon1 = locations[fromIndex]->getLongitude();
            double lat2 = locations[toIndex]->getLatitude();
            double lon2 = locations[toIndex]->getLongitude();
            dist = haversine(lat1, lon1, lat2, lon2);
        }

        connections.push_back({fromIndex, toIndex});
        distances.push_back(dist);
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
    if (!locations.empty()) {
        Location* loc = locations[0];
        std::cout << "   Name: " << loc->getName() << "\\n";
        std::cout << "   Coordinates: (" << loc->getLatitude() 
                  << ", " << loc->getLongitude() << ")\\n\\n";
    }
    
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
    if (!locations.empty()) {
        Path path1(locations[0]);
        for (size_t i = 1; i < locations.size() && i < 3; ++i) {
            path1.addLocation(locations[i]);
        }
        
        Path path2(locations[0]);
        for (size_t i = 1; i < locations.size() && i < 3; ++i) {
            path2.addLocation(locations[i]);
        }
        
        std::cout << "   Path 1: ";
        path1.print();
        std::cout << "   Path 2: ";
        path2.print();
        
        Path combined = path1 + path2;  // Operator overloading
        std::cout << "   Combined (path1 + path2): ";
        combined.print();
        
        std::cout << "   path1 < path2? " << (path1 < path2 ? "Yes" : "No") << "\\n\\n";
    }
    
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
        buildConnectionData(locations, connections, distances);
        std::cout << "Loaded " << connections.size() << " path connections\n";
        
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
        
        // Choose two visible locations for a safety pathfinding test (avoid hidden turn nodes)
        std::string start;
        std::string end;
        for (Location* loc : locations) {
            if (loc->getDescription() == "[hidden]") continue;
            if (start.empty()) { start = loc->getName(); continue; }
            if (end.empty()) { end = loc->getName(); break; }
        }

        if (start.empty() || end.empty()) {
            std::cout << "Skipping pathfinding test: not enough visible locations\n\n";
        } else {
            std::cout << "Finding path from " << start << " to " << end << "\n\n";

            // Walking mode
            try {
                navigator.setNavigationMode(std::make_shared<WalkingMode>());
                Path walkPath = navigator.findPath(start, end);
                std::cout << "Walking Mode:\n";
                walkPath.print();
                std::cout << "Distance: " << walkPath.getTotalDistance() << "m\n";
                std::cout << "Time: " << navigator.getEstimatedTime() << " minutes\n\n";
            } catch (const std::exception& e) {
                std::cerr << "Walking path test failed: " << e.what() << std::endl;
            }

            // Cycling mode
            try {
                navigator.setNavigationMode(std::make_shared<CyclingMode>());
                Path cyclePath = navigator.findPath(start, end);
                std::cout << "Cycling Mode:\n";
                cyclePath.print();
                std::cout << "Distance: " << cyclePath.getTotalDistance() << "m\n";
                std::cout << "Time: " << navigator.getEstimatedTime() << " minutes\n\n";
            } catch (const std::exception& e) {
                std::cerr << "Cycling path test failed: " << e.what() << std::endl;
            }
        }
        
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