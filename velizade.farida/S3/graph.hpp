#ifndef GRAPH_HPP
#define GRAPH_HPP

#include <string>
#include <utility>
#include "vector.hpp"
#include "hashTable.hpp"

namespace velizade
{
  struct EdgeHash
  {
    std::size_t operator()(const std::pair<std::string, std::string>& p) const
    {
      boost::hash2::siphash_64 hasher;
      boost::hash2::hash_append(hasher, {}, p.first);
      boost::hash2::hash_append(hasher, {}, p.second);
      return hasher.result();
    }
  };

  struct EdgeEqual
  {
    bool operator()(const std::pair<std::string, std::string>& a, const std::pair<std::string, std::string>& b) const
    {
      return a.first == b.first && a.second == b.second;
    }
  };

  class Graph
  {
  public:
    Graph();

    void addVertex(const std::string& v);
    bool hasVertex(const std::string& v) const noexcept;

    void addEdge(const std::string& src, const std::string& dest, unsigned int weight);
    bool cutEdge(const std::string& src, const std::string& dest, unsigned int weight);

    Graph merge(const Graph& other) const;
    Graph extract(const Vector<std::string>& vertices) const;

    const Vector<std::string>& getVertexes() const noexcept;

    void getOutbound(const std::string& v, Vector<std::pair<std::string, unsigned int>>& out) const;
    void getInbound(const std::string& v, Vector<std::pair<std::string, unsigned int>>& out) const;

  private:
    Vector<std::string> vertexes_;
    HashTable<std::pair<std::string, std::string>, Vector<unsigned int>, EdgeHash, EdgeEqual> edges_;

    Vector<std::pair<std::string, unsigned int>>
    sortPairs(const Vector<std::pair<std::string, unsigned int>>& vec) const;

    void swap(Graph& other) noexcept;
  };
}

#endif
