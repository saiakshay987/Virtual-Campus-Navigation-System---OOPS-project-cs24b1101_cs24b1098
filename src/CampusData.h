/**
 * @file CampusData.h
 * @brief Contains GPS coordinates and path data for IIITDM Kancheepuram campus
 * 
 * Real GPS coordinates extracted from campus location data:
 * Main Campus: 12.8378745°N, 80.1370940°E
 * Campus Area: 51 acres, fully residential
 */

/**
 * @file CampusData.h
 * @brief Static campus data (buildings and paths) used by the demo.
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
    std::string buildingType;
};
const std::vector<BuildingInfo> BUILDINGS = {
    {"Main gate", 12.84066, 80.13693, "Main enterance to the institute\nCloses at 10:30 pm", "Entrance"},
    {"SBI ATM", 12.84031, 80.13540, "Nearest ATM to the campus", "Amenity"},
    {"East gate", 12.83936, 80.13905, "To access Temple,church and mosque", "Entrance"},
    {"sewage treatment plant", 12.83882, 80.13933, "No description available", "Amenity"},
    {"cafeteria", 12.83786, 80.13862, "You can buy food here\nfor the people who hate mess food", "Amenity"},
    {"Pems block D", 12.83820, 80.13789, "Student clubs", "General"},
    {"Pems block ABC", 12.83826, 80.13693, "Student clubs", "General"},
    {"Football ground", 12.83707, 80.13676, "Football ground", "Sports"},
    {"Cricket ground", 12.83738, 80.13782, "Cricket ground", "Sports"},
    {"Arjuna sports complex", 12.83640, 80.13778, "Sports complex with badminton,table tennis\n and gym access", "Sports"},
    {"Courts", 12.83609, 80.13719, "Basketball,volleyball,tennis,squash,\nkabaddi courts and calsthenics", "Sports"},
    {"Medical centre", 12.83620, 80.13655, "A 24/7 medical facility\nDoctot timings 4-7pm", "Amenity"},
    {"Ultimate store", 12.83583, 80.13619, "Place where you can find \neverything you need", "Amenity"},
    {"Banyan hostel", 12.83605, 80.13633, "Mtech and Mdes hostel for boys", "Hostel"},
    {"Lotus hostel", 12.83613, 80.13587, "Mtech and Mdes hostel for girls", "Hostel"},
    {"Gulmohar enclave", 12.83630, 80.13529, "Faculty headquarters", "Residential"},
    {"Ashwatha&Ashoka hostels", 12.83508, 80.13587, "Boys hostels", "Hostel"},
    {"Jasmine&Jasmine annex hostels", 12.83698, 80.13553, "Jasmine - Girls hostel for all years\nJasmine annex - Boys hostel ", "Hostel"},
    {"Akshaya mess", 12.83734, 80.13578, "Mess for all students", "Amenity"},
    {"Open air theatre", 12.83891, 80.13634, "Small events happen here ", "Amenity"},
    {"Academic block", 12.83882, 80.13694, "Lecture halls and place for all events happening", "Academic"},
    {"Sky bridge", 12.83878, 80.13745, "Bridge connecting academic block and laboratory complex", "Landmark"},
    {"Laboratory complex", 12.83884, 80.13789, "Place for all laboratories,faculty rooms and research centers\nand sometimes exam halls", "Academic"},
    {"Library", 12.83945, 80.13801, "place to study and can find books and resources", "Academic"},
    {"Admin blcok&Senate hall", 12.83955, 80.13732, "All administrative offices and Senate hall\n->Senate hall for seminars", "Administrative"},
    {"Flag pole", 12.84015, 80.13736, "Flag pole", "Landmark"},
    {"Pond", 12.84030, 80.13699, "Pond-Near flag pole", "Landmark"}
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
const std::vector<PathConnection> PATHS = {
    {"Main Gate ", "turn_01", 0.0},
    {"Main gate", "Flag pole", 0.0},
    {"turn_01", "Flag pole", 0.0},
    {"turn_01", "turn_02", 0.0},
    {"turn_02", "turn_03", 0.0},
    {"turn_02", "SBI ATM", 0.0},
    {"turn_03", "turn_04", 0.0},
    {"turn_04", "turn_05", 0.0},
    {"turn_04", "Pems block ABC", 0.0},
    {"Pems block ABC", "Pems block D", 0.0},
    {"Pems block ABC", "Academic block", 0.0},
    {"turn_04", "Open Air theatre", 0.0},
    {"Open Air theatre", "Academic block", 0.0},
    {"turn_06", "turn_07", 0.0},
    {"turn_05", "Gulmohar enclave", 0.0},
    {"Gulmohar enclave", "Ashwatha&Ashoka hostels", 0.0},
    {"turn_04", "mess", 0.0},
    {"turn_04", "turn_06", 0.0},
    {"turn_07", "Banayan hostel", 0.0},
    {"turn_07", "Lotus hostel", 0.0},
    {"turn_07", "Medical centre", 0.0},
    {"Medical centre", "courts", 0.0},
    {"courts", "Arjuna sports complex", 0.0},
    {"Ashwatha&Ashoka hostels", "turn_08", 0.0},
    {"Pems block D", "cafeteria", 0.0},
    {"cafeteria", "turn_09", 0.0},
    {"Academic block", "Sky bridge", 0.0},
    {"Sky bridge", "laboratory", 0.0},
    {"turn_02", "Adminblcok&Senate hall", 0.0},
    {"Academic block", "Adminblcok&Senate hall", 0.0},
    {"turn_06", "Football ground", 0.0},
    {"Football ground ", "cricket ground", 0.0},
    {"laboratory", "cricket ground", 0.0},
    {"turn_08", "Arjuna sports complex", 0.0},
    {"Arjuna sports complex", "cafeteria", 0.0},
    {"turn_07", "Ultimate store", 0.0},
    {"Ultimate store", "Ashwatha&Ashoka hostels", 0.0},
    {"turn_04", "Akshaya mess", 0.0},
    {"turn_05", "Jasmine&Jasmine annex hostels", 0.0},
    {"turn_06", "Jasmine&Jasmine annex hostels", 0.0},
    {"Akshaya mess", "Jasmine&Jasmine annex hostels", 0.0},
    {"Flag pole", "East gate", 0.0},
    {"East gate", "sewage treatment plant", 0.0},
    {"sewage treatment plant", "turn_09", 0.0},
    {"Banyan hostel", "turn_07", 0.0},
    {"Laboratory complex", "Sky bridge", 0.0},
    {"Laboratory complex", "Library", 0.0},
    {"Sky bridge", "Library", 0.0},
    {"Laboratory complex", "cricket ground", 0.0}
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
    // Find bounding box of campus (updated to match provided coordinates)
    double minLat = 12.83508, maxLat = 12.84066;
    double minLon = 80.13529, maxLon = 80.13933;
    
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
