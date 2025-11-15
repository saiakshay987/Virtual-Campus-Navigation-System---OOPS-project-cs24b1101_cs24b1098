/**
 * @file CampusData.h
 * @brief Contains GPS coordinates and path data for IIITDM Kancheepuram campus
 * 
 * Real GPS coordinates extracted from campus location data:
 * Main Campus: 12.8378745°N, 80.1370940°E
 * Campus Area: 51 acres, fully residential
 */

#ifndef CAMPUS_DATA_H
#define CAMPUS_DATA_H

#include <string>
#include <vector>
#include <map>

namespace CampusData {

/**
 * @struct BuildingInfo
 * @brief Structure to store building information with GPS coordinates
 */
struct BuildingInfo {
    std::string name;
    double latitude;
    double longitude;
    std::string description;
    std::string buildingType; // "Academic", "Hostel", "Amenity", "Administrative"
};

// Real GPS coordinates of IIITDM Kancheepuram buildings
const std::vector<BuildingInfo> BUILDINGS = {
    {"Main Gate", 12.839500, 80.136500, "Main entrance to the campus", "Entrance"},
    {"Academic Block", 12.838200, 80.137400, "Main academic building with classrooms", "Academic"},
    {"Admin Block", 12.838000, 80.136800, "Administrative offices", "Administrative"},
    {"Lab Complex", 12.837700, 80.137600, "Laboratory facilities", "Academic"},
    {"Hostel A", 12.837200, 80.136500, "Student hostel block A", "Hostel"},
    {"Hostel B", 12.836800, 80.136800, "Student hostel block B", "Hostel"},
    {"Hostel C", 12.836500, 80.137200, "Student hostel block C", "Hostel"},
    {"Hostel D", 12.836200, 80.137600, "Student hostel block D", "Hostel"},
    {"Mess", 12.837000, 80.138000, "Dining hall and cafeteria", "Amenity"},
    {"Auditorium", 12.838500, 80.136500, "Main auditorium for events", "Amenity"},
    {"Sports Complex", 12.836000, 80.136800, "Sports facilities including courts and grounds", "Amenity"},
    {"Library", 12.837900, 80.137100, "Knowledge Plaza - Central library", "Academic"},
    {"Lecture Hall Complex", 12.838300, 80.137000, "Additional lecture halls", "Academic"}
};

/**
 * @struct PathConnection
 * @brief Represents a bidirectional path between two buildings
 */
struct PathConnection {
    std::string from;
    std::string to;
    double distanceMeters;
};

// Logical path connections based on actual campus walkways
// Distances calculated using Haversine formula
const std::vector<PathConnection> PATHS = {
    {"Main Gate", "Auditorium", 111.19},
    {"Main Gate", "Admin Block", 169.93},
    {"Auditorium", "Academic Block", 103.12},
    {"Auditorium", "Lecture Hall Complex", 58.59},
    {"Admin Block", "Academic Block", 68.75},
    {"Academic Block", "Library", 46.59},
    {"Academic Block", "Lab Complex", 59.68},
    {"Library", "Lecture Hall Complex", 45.78},
    {"Lab Complex", "Mess", 89.10},
    {"Hostel A", "Admin Block", 94.72},
    {"Hostel A", "Hostel B", 55.10},
    {"Hostel B", "Hostel C", 54.71},
    {"Hostel C", "Hostel D", 54.71},
    {"Hostel D", "Mess", 98.96},
    {"Hostel C", "Sports Complex", 70.51},
    {"Hostel A", "Sports Complex", 137.34},
    {"Mess", "Lab Complex", 89.10},
    {"Sports Complex", "Hostel B", 88.96}
};

/**
 * @brief Get building index by name
 * @param name Building name to search
 * @return Index of building or -1 if not found
 */
inline int getBuildingIndex(const std::string& name) {
    for (size_t i = 0; i < BUILDINGS.size(); ++i) {
        if (BUILDINGS[i].name == name) {
            return static_cast<int>(i);
        }
    }
    return -1;
}

/**
 * @brief Convert GPS coordinates to screen coordinates
 * @param lat Latitude
 * @param lon Longitude
 * @param screenWidth Window width
 * @param screenHeight Window height
 * @return std::pair of (x, y) screen coordinates
 */
inline std::pair<float, float> gpsToScreen(double lat, double lon, 
                                           int screenWidth, int screenHeight) {
    // Find bounding box of campus
    double minLat = 12.836000, maxLat = 12.839500;
    double minLon = 80.136500, maxLon = 80.138000;
    
    // Add padding (10% on each side)
    const float PADDING = 0.1f;
    float usableWidth = screenWidth * (1.0f - 2 * PADDING);
    float usableHeight = screenHeight * (1.0f - 2 * PADDING);
    
    // Normalize coordinates to [0, 1] range
    float normalizedX = (lon - minLon) / (maxLon - minLon);
    float normalizedY = (maxLat - lat) / (maxLat - minLat); // Inverted Y for screen coords
    
    // Scale to screen with padding
    float x = normalizedX * usableWidth + screenWidth * PADDING;
    float y = normalizedY * usableHeight + screenHeight * PADDING;
    
    return {x, y};
}

} // namespace CampusData

#endif // CAMPUS_DATA_H
