#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <utility>
#include <functional>
#include "hash_table.hpp"
#include "vector.hpp"

namespace velizade
{

  using EdgeKey = std::pair<std::string, std::string>;
  using EdgeTable = HashTable<EdgeKey, Vector<unsigned long long>>;

  struct Graph
  {
    std::string name;
    Vector<std::string> vertices;
    EdgeTable edges;

    Graph();
    explicit Graph(const std::string& n);

    void addVertex(const std::string& v);
    void addEdge(const std::string& from, const std::string& to, unsigned long long weight);
    bool removeEdge(const std::string& from, const std::string& to, unsigned long long weight);
    bool hasVertex(const std::string& v) const;
  };

  using GraphTable = HashTable<std::string, Graph>;
  GraphTable& getGraphs();

  void loadGraphsFromFile(const std::string& filename);

  Vector<std::string> splitLine(const std::string& line);
  void sortStrings(Vector<std::string>& vec);
  void printLines(const Vector<std::string>& lines);

}

#endif
