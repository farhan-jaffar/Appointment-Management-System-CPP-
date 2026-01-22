#include "Graph.h"
#include <queue>
#include <limits>

void Graph::addEdge(const std::string& from, const std::string& to, int distance) {
    adjacencyList[from].push_back({to, distance});
    adjacencyList[to].push_back({from, distance}); // Since it's an undirected graph
}

void Graph::dijkstra(const std::string& start, std::unordered_map<std::string, int>& distances) const {
    distances.clear();
    std::unordered_map<std::string, bool> visited;
    
    // Initialize distances
    for (const auto& pair : adjacencyList) {
        distances[pair.first] = std::numeric_limits<int>::max();
    }
    distances[start] = 0;

    // Priority queue to get vertex with minimum distance
    std::priority_queue<std::pair<int, std::string>, 
                      std::vector<std::pair<int, std::string>>, 
                      std::greater<std::pair<int, std::string>>> pq;
    
    pq.push({0, start});

    while (!pq.empty()) {
        std::string current = pq.top().second;
        pq.pop();

        if (visited[current]) continue;
        visited[current] = true;

        auto it = adjacencyList.find(current);
        if (it != adjacencyList.end()) {
            for (const auto& neighbor : it->second) {
                std::string next = neighbor.first;
                int weight = neighbor.second;

                if (!visited[next] && distances[current] != std::numeric_limits<int>::max() && 
                    distances[current] + weight < distances[next]) {
                    distances[next] = distances[current] + weight;
                    pq.push({distances[next], next});
                }
            }
        }
    }
} 