#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <unordered_map>
#include <vector>

class Graph {
private:
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> adjacencyList;

public:
    void addEdge(const std::string& from, const std::string& to, int distance);
    void dijkstra(const std::string& start, std::unordered_map<std::string, int>& distances) const;
};

#endif 