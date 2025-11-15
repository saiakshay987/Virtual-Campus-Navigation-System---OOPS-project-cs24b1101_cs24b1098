/**
 * @file Navigator.cpp
 * @brief Implementation of Navigator class with Dijkstra's algorithm
 */

#include "Navigator.h"
#include "WalkingMode.h"
#include <queue>
#include <limits>
#include <algorithm>
#include <iostream>

// Constants
const double INF = std::numeric_limits<double>::infinity();

// Constructor
Navigator::Navigator() {
    // Set default navigation mode to walking
    currentMode_ = std::make_shared<WalkingMode>();
}

// Destructor
Navigator::~Navigator() {
    // Clean up if needed
}

// Initialize graph
void Navigator::initializeGraph(const std::vector<Location*>& locations,
                                 const std::vector<std::pair<int, int>>& connections,
                                 const std::vector<double>& distances) {
    // Store all locations
    allLocations_ = locations;
    
    // Add all locations as nodes
    for (Location* loc : locations) {
        graph_.addNode(loc);
    }
    
    // Add edges based on connections
    for (size_t i = 0; i < connections.size(); ++i) {
        int from = connections[i].first;
        int to = connections[i].second;
        double dist = distances[i];
        
        // Add bidirectional edge
        graph_.addUndirectedEdge(locations[from], locations[to], dist);
    }
}

// Find path by name
Path Navigator::findPath(const std::string& startName, const std::string& endName) {
    // EXCEPTION HANDLING: Validate inputs
    if (startName.empty() || endName.empty()) {
        throw InvalidLocationException("Location names cannot be empty");
    }
    
    Location* start = getLocationByName(startName);
    Location* end = getLocationByName(endName);
    
    return findPath(start, end);
}

// Find path by pointers
Path Navigator::findPath(Location* start, Location* end) {
    // EXCEPTION HANDLING: Validate inputs
    if (start == nullptr || end == nullptr) {
        throw InvalidLocationException("Start or end location is null");
    }
    
    if (!graph_.hasNode(start) || !graph_.hasNode(end)) {
        throw InvalidLocationException("Location not found in graph");
    }
    
    // Use Dijkstra's algorithm (ABSTRACTION: hidden from public interface)
    lastPath_ = dijkstraShortestPath(start, end);
    
    return lastPath_;
}

/**
 * @brief Dijkstra's Algorithm Implementation
 * 
 * Time Complexity: O((V + E) log V) using priority queue
 * Space Complexity: O(V)
 * 
 * Algorithm Steps:
 * 1. Initialize all distances to infinity except source (0)
 * 2. Use priority queue to always process closest unvisited node
 * 3. For each neighbor, calculate tentative distance
 * 4. Update if shorter path found
 * 5. Reconstruct path by backtracking
 */
Path Navigator::dijkstraShortestPath(Location* start, Location* end) {
    // Data structures for Dijkstra
    std::map<Location*, double> distances;
    std::map<Location*, Location*> previous;
    std::map<Location*, bool> visited;
    
    // Priority queue: pair<distance, location>
    // Min-heap based on distance
    std::priority_queue<std::pair<double, Location*>,
                        std::vector<std::pair<double, Location*>>,
                        std::greater<std::pair<double, Location*>>> pq;
    
    // Step 1: Initialize distances
    for (Location* loc : allLocations_) {
        distances[loc] = INF;
        visited[loc] = false;
    }
    distances[start] = 0.0;
    
    // Step 2: Add start node to priority queue
    pq.push({0.0, start});
    
    // Step 3: Main Dijkstra loop
    while (!pq.empty()) {
        // Get node with minimum distance
        Location* current = pq.top().second;
        double currentDist = pq.top().first;
        pq.pop();
        
        // Skip if already visited
        if (visited[current]) {
            continue;
        }
        visited[current] = true;
        
        // Early termination if destination reached
        if (current == end) {
            break;
        }
        
        // Check all neighbors
        std::vector<Edge<Location*>> neighbors = graph_.getNeighbors(current);
        
        for (const Edge<Location*>& edge : neighbors) {
            Location* neighbor = edge.destination;
            double edgeWeight = edge.weight;
            
            // Calculate tentative distance
            double tentativeDist = currentDist + edgeWeight;
            
            // Update if shorter path found
            if (tentativeDist < distances[neighbor]) {
                distances[neighbor] = tentativeDist;
                previous[neighbor] = current;
                pq.push({tentativeDist, neighbor});
            }
        }
    }
    
    // Step 4: Check if path exists
    if (distances[end] == INF) {
        throw PathNotFoundException(
            "No path exists between " + start->getName() + 
            " and " + end->getName()
        );
    }
    
    // Step 5: Reconstruct path
    return reconstructPath(start, end, previous, distances);
}

// Reconstruct path from Dijkstra results
Path Navigator::reconstructPath(Location* start, Location* end,
                                 const std::map<Location*, Location*>& previous,
                                 const std::map<Location*, double>& distances) {
    Path path;
    
    // Backtrack from end to start
    std::vector<Location*> reversePath;
    Location* current = end;
    
    while (current != start) {
        reversePath.push_back(current);
        auto it = previous.find(current);
        if (it == previous.end()) {
            throw PathNotFoundException("Path reconstruction failed");
        }
        current = it->second;
    }
    reversePath.push_back(start);
    
    // Reverse to get correct order
    std::reverse(reversePath.begin(), reversePath.end());
    
    // Build path object
    for (Location* loc : reversePath) {
        if (path.empty()) {
            path = Path(loc);
        } else {
            path.addLocation(loc);
        }
    }
    
    // Set total distance from Dijkstra result
    path.setTotalDistance(distances.at(end));
    
    return path;
}

// Set navigation mode
void Navigator::setNavigationMode(std::shared_ptr<NavigationMode> mode) {
    if (mode == nullptr) {
        throw std::invalid_argument("Navigation mode cannot be null");
    }
    currentMode_ = mode;
}

// Get navigation mode
std::shared_ptr<NavigationMode> Navigator::getNavigationMode() const {
    return currentMode_;
}

// Calculate estimated time
double Navigator::getEstimatedTime() const {
    if (lastPath_.empty()) {
        return 0.0;
    }
    
    if (currentMode_ == nullptr) {
        throw std::runtime_error("Navigation mode not set");
    }
    
    return currentMode_->calculateTime(lastPath_.getTotalDistance());
}

// Get location by name
Location* Navigator::getLocationByName(const std::string& name) {
    for (Location* loc : allLocations_) {
        if (loc->getName() == name) {
            return loc;
        }
    }
    
    throw InvalidLocationException("Location '" + name + "' not found");
}

// Get all locations
std::vector<Location*> Navigator::getAllLocations() const {
    return allLocations_;
}

// Get last path
Path Navigator::getLastPath() const {
    return lastPath_;
}
