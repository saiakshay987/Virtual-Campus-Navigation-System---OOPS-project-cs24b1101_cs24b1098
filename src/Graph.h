/**
 * @file Graph.h
 * @brief Template graph class demonstrating TEMPLATES
 * 
 * OOP Concept: Templates (Generic Programming)
 * - Template class that works with any node type
 * - Generic graph implementation
 * - Type-safe and reusable
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <map>
#include <vector>
#include <stdexcept>
#include <algorithm>

/**
 * @struct Edge
 * @brief Represents a weighted edge in the graph
 * 
 * TEMPLATE: Generic edge structure
 */
template<typename T>
struct Edge {
    T destination;      ///< Destination node
    double weight;      ///< Edge weight (distance)
    
    Edge(T dest, double w) : destination(dest), weight(w) {}
};

/**
 * @class Graph
 * @brief Generic graph class using templates
 * 
 * Demonstrates TEMPLATES by:
 * - Working with any data type T
 * - Providing generic graph operations
 * - Type-safe implementation
 * 
 * Example usage:
 * @code
 * Graph<Location*> campusGraph;
 * campusGraph.addNode(loc1);
 * campusGraph.addEdge(loc1, loc2, 100.5);
 * @endcode
 */
template<typename T>
class Graph {
private:
    // Adjacency list representation: node -> list of edges
    std::map<T, std::vector<Edge<T>>> adjacencyList_;
    
public:
    /**
     * @brief Default constructor
     */
    Graph() {}
    
    /**
     * @brief Add a node to the graph
     * @param node Node to add
     */
    void addNode(T node) {
        if (adjacencyList_.find(node) == adjacencyList_.end()) {
            adjacencyList_[node] = std::vector<Edge<T>>();
        }
    }
    
    /**
     * @brief Add a directed edge
     * @param from Source node
     * @param to Destination node
     * @param weight Edge weight
     */
    void addEdge(T from, T to, double weight) {
        // Ensure both nodes exist
        addNode(from);
        addNode(to);
        
        // Add edge
        adjacencyList_[from].push_back(Edge<T>(to, weight));
    }
    
    /**
     * @brief Add an undirected edge (bidirectional)
     * @param node1 First node
     * @param node2 Second node
     * @param weight Edge weight
     */
    void addUndirectedEdge(T node1, T node2, double weight) {
        addEdge(node1, node2, weight);
        addEdge(node2, node1, weight);
    }
    
    /**
     * @brief Get all neighbors of a node
     * @param node Node to query
     * @return Vector of edges to neighbors
     */
    std::vector<Edge<T>> getNeighbors(T node) const {
        auto it = adjacencyList_.find(node);
        if (it != adjacencyList_.end()) {
            return it->second;
        }
        return std::vector<Edge<T>>();
    }
    
    /**
     * @brief Check if node exists in graph
     * @param node Node to check
     * @return True if node exists
     */
    bool hasNode(T node) const {
        return adjacencyList_.find(node) != adjacencyList_.end();
    }
    
    /**
     * @brief Check if edge exists
     * @param from Source node
     * @param to Destination node
     * @return True if edge exists
     */
    bool hasEdge(T from, T to) const {
        auto it = adjacencyList_.find(from);
        if (it == adjacencyList_.end()) {
            return false;
        }
        
        for (const Edge<T>& edge : it->second) {
            if (edge.destination == to) {
                return true;
            }
        }
        return false;
    }
    
    /**
     * @brief Get edge weight
     * @param from Source node
     * @param to Destination node
     * @return Edge weight
     * @throws std::runtime_error if edge doesn't exist
     */
    double getEdgeWeight(T from, T to) const {
        auto it = adjacencyList_.find(from);
        if (it == adjacencyList_.end()) {
            throw std::runtime_error("Source node not found in graph");
        }
        
        for (const Edge<T>& edge : it->second) {
            if (edge.destination == to) {
                return edge.weight;
            }
        }
        
        throw std::runtime_error("Edge not found in graph");
    }
    
    /**
     * @brief Get all nodes in graph
     * @return Vector of all nodes
     */
    std::vector<T> getAllNodes() const {
        std::vector<T> nodes;
        for (const auto& pair : adjacencyList_) {
            nodes.push_back(pair.first);
        }
        return nodes;
    }
    
    /**
     * @brief Get number of nodes
     * @return Number of nodes
     */
    size_t getNodeCount() const {
        return adjacencyList_.size();
    }
    
    /**
     * @brief Get number of edges
     * @return Number of edges
     */
    size_t getEdgeCount() const {
        size_t count = 0;
        for (const auto& pair : adjacencyList_) {
            count += pair.second.size();
        }
        return count;
    }
    
    /**
     * @brief Clear the graph
     */
    void clear() {
        adjacencyList_.clear();
    }
    
    /**
     * @brief Remove a node from graph
     * @param node Node to remove
     */
    void removeNode(T node) {
        adjacencyList_.erase(node);
        
        // Remove all edges pointing to this node
        for (auto& pair : adjacencyList_) {
            auto& edges = pair.second;
            edges.erase(
                std::remove_if(edges.begin(), edges.end(),
                    [node](const Edge<T>& e) { return e.destination == node; }),
                edges.end()
            );
        }
    }
    
    /**
     * @brief Remove an edge from graph
     * @param from Source node
     * @param to Destination node
     */
    void removeEdge(T from, T to) {
        auto it = adjacencyList_.find(from);
        if (it != adjacencyList_.end()) {
            auto& edges = it->second;
            edges.erase(
                std::remove_if(edges.begin(), edges.end(),
                    [to](const Edge<T>& e) { return e.destination == to; }),
                edges.end()
            );
        }
    }
};

#endif // GRAPH_H
