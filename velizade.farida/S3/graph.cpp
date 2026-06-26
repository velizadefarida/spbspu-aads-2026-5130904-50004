#include "graph.hpp"
#include <algorithm>

velizade::Graph::Graph():
    vertexes_(),
    edges_(16, 4, 4)
{}

void velizade::Graph::addVertex(const std::string& v)
{
  size_t left = 0, right = vertexes_.getSize();
  while (left < right)
  {
    size_t mid = left + (right - left) / 2;
    if (vertexes_[mid] < v)
    {
      left = mid + 1;
    }
    else
    {
      right = mid;
    }
  }
  if (left < vertexes_.getSize() && vertexes_[left] == v)
  {
    return;
  }

  Vector<std::string> newV = vertexes_;
  newV.insert(left, v);
  vertexes_.swap(newV);
}

bool velizade::Graph::hasVertex(const std::string& v) const noexcept
{
  size_t left = 0, right = vertexes_.getSize();
  while (left < right)
  {
    size_t mid = left + (right - left) / 2;
    if (vertexes_[mid] < v)
    {
      left = mid + 1;
    }
    else
    {
      right = mid;
    }
  }
  return left < vertexes_.getSize() && vertexes_[left] == v;
}

void velizade::Graph::addEdge(const std::string& src, const std::string& dest, unsigned int weight)
{
  Graph copy = *this;
  copy.addVertex(src);
  copy.addVertex(dest);

  auto key = std::make_pair(src, dest);
  auto it = copy.edges_.find(key);
  if (it != copy.edges_.end())
  {
    Vector<unsigned int> weights = it->second;
    weights.pushBack(weight);
    copy.edges_.add(key, weights);
  }
  else
  {
    Vector<unsigned int> weights;
    weights.pushBack(weight);
    copy.edges_.add(key, weights);
  }

  swap(copy);
}

bool velizade::Graph::cutEdge(const std::string& src, const std::string& dest, unsigned int weight)
{
  auto key = std::make_pair(src, dest);
  auto it = edges_.find(key);
  if (it == edges_.end())
  {
    return false;
  }

  Graph copy = *this;
  auto it2 = copy.edges_.find(key);
  Vector<unsigned int> weights = it2->second;
  for (size_t i = 0; i < weights.getSize(); ++i)
  {
    if (weights[i] == weight)
    {
      weights.erase(i);
      if (weights.isEmpty())
      {
        copy.edges_.drop(key);
      }
      else
      {
        copy.edges_.add(key, weights);
      }
      swap(copy);
      return true;
    }
  }
  return false;
}

velizade::Graph velizade::Graph::merge(const Graph& other) const
{
  Graph res = *this;
  for (size_t i = 0; i < other.vertexes_.getSize(); ++i)
  {
    res.addVertex(other.vertexes_[i]);
  }

  for (auto it = other.edges_.cbegin(); it != other.edges_.cend(); ++it)
  {
    const auto& key = it->first;
    const auto& weights = it->second;
    for (size_t i = 0; i < weights.getSize(); ++i)
    {
      res.addEdge(key.first, key.second, weights[i]);
    }
  }
  return res;
}

velizade::Graph velizade::Graph::extract(const Vector<std::string>& vertices) const
{
  Graph res;
  for (size_t i = 0; i < vertices.getSize(); ++i)
  {
    res.addVertex(vertices[i]);
  }

  for (auto it = edges_.cbegin(); it != edges_.cend(); ++it)
  {
    const auto& key = it->first;
    if (res.hasVertex(key.first) && res.hasVertex(key.second))
    {
      const auto& weights = it->second;
      for (size_t i = 0; i < weights.getSize(); ++i)
      {
        res.addEdge(key.first, key.second, weights[i]);
      }
    }
  }
  return res;
}

const velizade::Vector<std::string>& velizade::Graph::getVertexes() const noexcept
{
  return vertexes_;
}

void velizade::Graph::getOutbound(const std::string& v, Vector<std::pair<std::string, unsigned int>>& out) const
{
  Vector<std::pair<std::string, unsigned int>> temp;
  for (auto it = edges_.cbegin(); it != edges_.cend(); ++it)
  {
    if (it->first.first == v)
    {
      const auto& weights = it->second;
      for (size_t i = 0; i < weights.getSize(); ++i)
      {
        temp.pushBack(std::make_pair(it->first.second, weights[i]));
      }
    }
  }
  out = sortPairs(temp);
}

void velizade::Graph::getInbound(const std::string& v, Vector<std::pair<std::string, unsigned int>>& out) const
{
  Vector<std::pair<std::string, unsigned int>> temp;
  for (auto it = edges_.cbegin(); it != edges_.cend(); ++it)
  {
    if (it->first.second == v)
    {
      const auto& weights = it->second;
      for (size_t i = 0; i < weights.getSize(); ++i)
      {
        temp.pushBack(std::make_pair(it->first.first, weights[i]));
      }
    }
  }
  out = sortPairs(temp);
}

velizade::Vector<std::pair<std::string, unsigned int>>
velizade::Graph::sortPairs(const Vector<std::pair<std::string, unsigned int>>& vec) const
{
  Vector<std::pair<std::string, unsigned int>> copy = vec;
  for (size_t i = 0; i < copy.getSize(); ++i)
  {
    for (size_t j = i + 1; j < copy.getSize(); ++j)
    {
      if (copy[j].first < copy[i].first ||
          (copy[j].first == copy[i].first && copy[j].second < copy[i].second))
      {
        std::swap(copy[i], copy[j]);
      }
    }
  }
  return copy;
}

void velizade::Graph::swap(Graph& other) noexcept
{
  vertexes_.swap(other.vertexes_);
  edges_.swap(other.edges_);
}
