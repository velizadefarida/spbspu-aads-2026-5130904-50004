#include "graph.hpp"
#include <algorithm>

velizade::Graph::Graph() :
  vertices_(),
  edges_(16, 4)
{}

void velizade::Graph::addVertex(const std::string& v)
{
  for (size_t i = 0; i < vertices_.getSize(); ++i)
  {
    if (vertices_[i] == v) return;
  }
  vertices_.pushBack(v);
}

bool velizade::Graph::hasVertex(const std::string& v) const
{
  for (size_t i = 0; i < vertices_.getSize(); ++i)
  {
    if (vertices_[i] == v) return true;
  }
  return false;
}

void velizade::Graph::addEdge(const std::string& from, const std::string& to, unsigned long long weight)
{
  addVertex(from);
  addVertex(to);
  EdgeKey key = {from, to};
  if (edges_.has(key))
  {
    auto* cell = edges_.find(key);
    if (cell)
    {
      cell->value.pushBack(weight);
    }
  }
  else
  {
    Vector<unsigned long long> weights;
    weights.pushBack(weight);
    edges_.add(key, weights);
  }
}

bool velizade::Graph::removeEdge(const std::string& from, const std::string& to, unsigned long long weight)
{
  if (!hasVertex(from) || !hasVertex(to)) return false;
  EdgeKey key = {from, to};
  auto* cell = edges_.find(key);
  if (!cell) return false;
  auto& vec = cell->value;
  for (size_t i = 0; i < vec.getSize(); ++i)
  {
    if (vec[i] == weight)
    {
      vec.erase(i);
      if (vec.isEmpty()) edges_.drop(key);
      return true;
    }
  }
  return false;
}

velizade::Vector<std::string> velizade::Graph::getVertexes() const
{
  return vertices_;
}

void velizade::Graph::getOutbound(const std::string& v, Vector<std::pair<std::string, unsigned long long>>& out) const
{
  Vector<std::pair<std::string, unsigned long long>> temp;
  for (auto it = edges_.begin(); it != edges_.end(); ++it)
  {
    auto kv = *it;
    if (kv.first.first == v)
    {
      for (size_t i = 0; i < kv.second.getSize(); ++i)
      {
        temp.pushBack({kv.first.second, kv.second[i]});
      }
    }
  }
  out = sortPairs(temp);
}

void velizade::Graph::getInbound(const std::string& v, Vector<std::pair<std::string, unsigned long long>>& out) const
{
  Vector<std::pair<std::string, unsigned long long>> temp;
  for (auto it = edges_.begin(); it != edges_.end(); ++it)
  {
    auto kv = *it;
    if (kv.first.second == v)
    {
      for (size_t i = 0; i < kv.second.getSize(); ++i)
      {
        temp.pushBack({kv.first.first, kv.second[i]});
      }
    }
  }
  out = sortPairs(temp);
}

velizade::Graph velizade::Graph::merge(const Graph& other) const
{
  Graph result;
  for (size_t i = 0; i < vertices_.getSize(); ++i)
  {
    result.addVertex(vertices_[i]);
  }
  for (size_t i = 0; i < other.vertices_.getSize(); ++i)
  {
    result.addVertex(other.vertices_[i]);
  }
  for (auto it = edges_.begin(); it != edges_.end(); ++it)
  {
    auto kv = *it;
    for (size_t i = 0; i < kv.second.getSize(); ++i)
    {
      result.addEdge(kv.first.first, kv.first.second, kv.second[i]);
    }
  }
  for (auto it = other.edges_.begin(); it != other.edges_.end(); ++it)
  {
    auto kv = *it;
    for (size_t i = 0; i < kv.second.getSize(); ++i)
    {
      result.addEdge(kv.first.first, kv.first.second, kv.second[i]);
    }
  }
  return result;
}

velizade::Graph velizade::Graph::extract(const Vector<std::string>& vertices) const
{
  Graph result;
  for (size_t i = 0; i < vertices.getSize(); ++i)
  {
    if (hasVertex(vertices[i]))
    {
      result.addVertex(vertices[i]);
    }
  }
  for (auto it = edges_.begin(); it != edges_.end(); ++it)
  {
    auto kv = *it;
    const std::string& from = kv.first.first;
    const std::string& to = kv.first.second;
    if (result.hasVertex(from) && result.hasVertex(to))
    {
      for (size_t i = 0; i < kv.second.getSize(); ++i)
      {
        result.addEdge(from, to, kv.second[i]);
      }
    }
  }
  return result;
}

velizade::Vector<std::pair<std::string, unsigned long long>>
velizade::Graph::sortPairs(const Vector<std::pair<std::string, unsigned long long>>& vec) const
{
  Vector<std::pair<std::string, unsigned long long>> copy = vec;
  std::sort(copy.begin(), copy.end(),
    [](const auto& a, const auto& b) {
      if (a.first != b.first) return a.first < b.first;
      return a.second < b.second;
    });
  return copy;
}
