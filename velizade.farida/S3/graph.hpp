#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <utility>
#include "hash_table.hpp"
#include "vector.hpp"

namespace velizade
{

  using EdgeKey = std::pair<std::string, std::string>;
  using EdgeTable = HashTable<EdgeKey, Vector<unsigned long long>>;

  struct Graph
  {
    Graph();

    void addVertex(const std::string& v);
    bool hasVertex(const std::string& v) const;
    void addEdge(const std::string& from, const std::string& to, unsigned long long weight);
    bool removeEdge(const std::string& from, const std::string& to, unsigned long long weight);

    Vector<std::string> getVertexes() const;
    void getOutbound(const std::string& v, Vector<std::pair<std::string, unsigned long long>>& out) const;
    void getInbound(const std::string& v, Vector<std::pair<std::string, unsigned long long>>& out) const;

    Graph merge(const Graph& other) const;
    Graph extract(const Vector<std::string>& vertices) const;

  private:
    Vector<std::string> vertices_;
    EdgeTable edges_;

    Vector<std::pair<std::string, unsigned long long>>
      sortPairs(const Vector<std::pair<std::string, unsigned long long>>& vec) const;
  };

}

#endif
