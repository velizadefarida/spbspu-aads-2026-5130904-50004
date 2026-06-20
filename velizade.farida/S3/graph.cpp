#include "graph.hpp"
#include <fstream>
#include <algorithm>
#include <stdexcept>
#include <iostream>

velizade::Graph::Graph() :
  edges(16, 4)
{}

velizade::Graph::Graph(const std::string& n) :
  name(n),
  edges(16, 4)
{}

void velizade::Graph::addVertex(const std::string& v)
{
  for (size_t i = 0; i < vertices.getSize(); ++i)
  {
    if (vertices[i] == v)
    {
      return;
    }
  }
  vertices.pushBack(v);
}

void velizade::Graph::addEdge(const std::string& from, const std::string& to, unsigned long long weight)
{
  EdgeKey key = {from, to};
  if (edges.has(key))
  {
    auto* cell = edges.find(key);
    if (cell)
    {
      cell->value.pushBack(weight);
    }
    else
    {
      throw std::runtime_error("Internal error");
    }
  }
  else
  {
    Vector<unsigned long long> weights;
    weights.pushBack(weight);
    edges.add(key, weights);
  }
}

bool velizade::Graph::removeEdge(const std::string& from, const std::string& to, unsigned long long weight)
{
  EdgeKey key = {from, to};
  auto* cell = edges.find(key);
  if (!cell)
  {
    return false;
  }
  auto& vec = cell->value;
  for (size_t i = 0; i < vec.getSize(); ++i)
  {
    if (vec[i] == weight)
    {
      vec.erase(i);
      if (vec.isEmpty())
      {
        edges.drop(key);
      }
      return true;
    }
  }
  return false;
}

bool velizade::Graph::hasVertex(const std::string& v) const
{
  for (size_t i = 0; i < vertices.getSize(); ++i)
  {
    if (vertices[i] == v)
    {
      return true;
    }
  }
  return false;
}

velizade::GraphTable& velizade::getGraphs()
{
  static GraphTable graphs(64, 4);
  return graphs;
}

void velizade::loadGraphsFromFile(const std::string& filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open file");
  }

  auto& graphs = getGraphs();
  std::string gname;
  size_t edgesCount;
  while (file >> gname >> edgesCount)
  {
    Graph g(gname);
    for (size_t i = 0; i < edgesCount; ++i)
    {
      std::string from, to;
      unsigned long long weight;
      file >> from >> to >> weight;
      g.addVertex(from);
      g.addVertex(to);
      g.addEdge(from, to, weight);
    }
    if (graphs.has(gname))
    {
      throw std::runtime_error("Duplicate graph name in file");
    }
    graphs.add(gname, std::move(g));
  }
}

velizade::Vector<std::string> velizade::splitLine(const std::string& line)
{
  Vector<std::string> tokens;
  size_t start = 0;
  while (start < line.size())
  {
    while (start < line.size() && (line[start] == ' ' || line[start] == '\t'))
    {
      ++start;
    }
    if (start >= line.size())
    {
      break;
    }
    size_t end = start;
    while (end < line.size() && line[end] != ' ' && line[end] != '\t')
    {
      ++end;
    }
    tokens.pushBack(line.substr(start, end - start));
    start = end;
  }
  return tokens;
}

void velizade::sortStrings(Vector<std::string>& vec)
{
  std::sort(vec.begin(), vec.end());
}

void velizade::printLines(const Vector<std::string>& lines)
{
  for (size_t i = 0; i < lines.getSize(); ++i)
  {
    std::cout << lines[i] << "\n";
  }
}
