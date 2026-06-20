#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

namespace velizade
{

  template<class T>
  void sortVector(Vector<T>& vec)
  {
    std::sort(vec.begin(), vec.end());
  }

  Vector<std::string> splitString(const std::string& str)
  {
    Vector<std::string> res;
    std::string current;
    for (size_t i = 0; i < str.size(); ++i)
    {
      if (str[i] == ' ' || str[i] == '\r' || str[i] == '\n' || str[i] == '\t')
      {
        if (!current.empty())
        {
          res.pushBack(current);
          current.clear();
        }
      }
      else
      {
        current += str[i];
      }
    }
    if (!current.empty())
    {
      res.pushBack(current);
    }
    return res;
  }

  bool tryParseUInt(const std::string& str, unsigned long long& out)
  {
    if (str.empty()) return false;
    unsigned long long res = 0;
    for (size_t i = 0; i < str.size(); ++i)
    {
      if (str[i] < '0' || str[i] > '9') return false;
      res = res * 10 + (str[i] - '0');
    }
    out = res;
    return true;
  }

  void throwInvalid()
  {
    throw std::invalid_argument("INVALID COMMAND");
  }

}

velizade::Application::Application() :
  commands_(16),
  graphs_(16)
{
  commands_.add("graphs", &Application::cmdGraphs);
  commands_.add("vertexes", &Application::cmdVertexes);
  commands_.add("outbound", &Application::cmdOutbound);
  commands_.add("inbound", &Application::cmdInbound);
  commands_.add("bind", &Application::cmdBind);
  commands_.add("cut", &Application::cmdCut);
  commands_.add("create", &Application::cmdCreate);
  commands_.add("merge", &Application::cmdMerge);
  commands_.add("extract", &Application::cmdExtract);
}

void velizade::Application::loadFromFile(const std::string& filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open file");
  }

  std::string line;
  while (std::getline(file, line))
  {
    Vector<std::string> tokens = splitString(line);
    if (tokens.isEmpty()) continue;
    if (tokens.getSize() == 2)
    {
      std::string graphName = tokens[0];
      unsigned long long edgeCount = 0;
      if (!tryParseUInt(tokens[1], edgeCount)) continue;
      Graph g;
      for (size_t i = 0; i < edgeCount; ++i)
      {
        std::string edgeLine;
        if (std::getline(file, edgeLine))
        {
          Vector<std::string> edgeTokens = splitString(edgeLine);
          if (edgeTokens.getSize() >= 3)
          {
            std::string src = edgeTokens[0];
            std::string dest = edgeTokens[1];
            unsigned long long weight;
            if (tryParseUInt(edgeTokens[2], weight))
            {
              g.addEdge(src, dest, weight);
            }
          }
        }
      }
      if (graphs_.has(graphName))
      {
        throw std::runtime_error("Duplicate graph name");
      }
      graphs_.add(graphName, g);
    }
  }
}

void velizade::Application::run(const std::string& filename)
{
  loadFromFile(filename);
  std::string line;
  while (std::getline(std::cin, line))
  {
    processLine(line);
  }
}

void velizade::Application::processLine(const std::string& line)
{
  Vector<std::string> tokens = splitString(line);
  if (tokens.isEmpty()) return;
  auto* cell = commands_.find(tokens[0]);
  if (cell)
  {
    try
    {
      cell->value(this, tokens);
    }
    catch (const std::exception&)
    {
      std::cout << "<INVALID COMMAND>\n";
    }
  }
  else
  {
    std::cout << "<INVALID COMMAND>\n";
  }
}

void velizade::Application::cmdGraphs(Application* app, const Vector<std::string>& args)
{
  if (args.getSize() != 1) throwInvalid();
  Vector<std::string> names;
  for (auto it = app->graphs_.begin(); it != app->graphs_.end(); ++it)
  {
    auto kv = *it;
    names.pushBack(kv.first);
  }
  if (names.getSize() == 0)
  {
    std::cout << '\n';
    return;
  }
  sortVector(names);
  for (size_t i = 0; i < names.getSize(); ++i)
  {
    std::cout << names[i] << '\n';
  }
}

void velizade::Application::cmdVertexes(Application* app, const Vector<std::string>& args)
{
  if (args.getSize() != 2) throwInvalid();
  auto* cell = app->graphs_.find(args[1]);
  if (!cell) throwInvalid();
  Vector<std::string> verts = cell->value.getVertexes();
  if (verts.getSize() == 0)
  {
    std::cout << '\n';
    return;
  }
  sortVector(verts);
  for (size_t i = 0; i < verts.getSize(); ++i)
  {
    std::cout << verts[i] << '\n';
  }
}

void velizade::Application::cmdOutbound(Application* app, const Vector<std::string>& args)
{
  if (args.getSize() != 3) throwInvalid();
  auto* cell = app->graphs_.find(args[1]);
  if (!cell || !cell->value.hasVertex(args[2])) throwInvalid();
  Vector<std::pair<std::string, unsigned long long>> res;
  cell->value.getOutbound(args[2], res);
  if (res.getSize() == 0)
  {
    std::cout << '\n';
    return;
  }
  std::string currentV = res[0].first;
  std::cout << currentV << ' ' << res[0].second;
  for (size_t i = 1; i < res.getSize(); ++i)
  {
    if (res[i].first == currentV)
    {
      std::cout << ' ' << res[i].second;
    }
    else
    {
      currentV = res[i].first;
      std::cout << '\n' << currentV << ' ' << res[i].second;
    }
  }
  std::cout << '\n';
}

void velizade::Application::cmdInbound(Application* app, const Vector<std::string>& args)
{
  if (args.getSize() != 3) throwInvalid();
  auto* cell = app->graphs_.find(args[1]);
  if (!cell || !cell->value.hasVertex(args[2])) throwInvalid();
  Vector<std::pair<std::string, unsigned long long>> res;
  cell->value.getInbound(args[2], res);
  if (res.getSize() == 0)
  {
    std::cout << '\n';
    return;
  }
  std::string currentV = res[0].first;
  std::cout << currentV << ' ' << res[0].second;
  for (size_t i = 1; i < res.getSize(); ++i)
  {
    if (res[i].first == currentV)
    {
      std::cout << ' ' << res[i].second;
    }
    else
    {
      currentV = res[i].first;
      std::cout << '\n' << currentV << ' ' << res[i].second;
    }
  }
  std::cout << '\n';
}

void velizade::Application::cmdBind(Application* app, const Vector<std::string>& args)
{
  unsigned long long weight = 0;
  if (args.getSize() != 5 || !tryParseUInt(args[4], weight)) throwInvalid();
  auto* cell = app->graphs_.find(args[1]);
  if (!cell) throwInvalid();
  cell->value.addEdge(args[2], args[3], weight);
}

void velizade::Application::cmdCut(Application* app, const Vector<std::string>& args)
{
  unsigned long long weight = 0;
  if (args.getSize() != 5 || !tryParseUInt(args[4], weight)) throwInvalid();
  auto* cell = app->graphs_.find(args[1]);
  if (!cell || !cell->value.removeEdge(args[2], args[3], weight)) throwInvalid();
}

void velizade::Application::cmdCreate(Application* app, const Vector<std::string>& args)
{
  unsigned long long count = 0;
  if (args.getSize() < 3 || !tryParseUInt(args[2], count) || args.getSize() != 3 + count) throwInvalid();
  if (app->graphs_.has(args[1])) throwInvalid();
  for (size_t i = 0; i < count; ++i)
  {
    for (size_t j = i + 1; j < count; ++j)
    {
      if (args[3 + i] == args[3 + j]) throwInvalid();
    }
  }
  Graph newG;
  for (size_t i = 0; i < count; ++i)
  {
    newG.addVertex(args[3 + i]);
  }
  app->graphs_.add(args[1], newG);
}

void velizade::Application::cmdMerge(Application* app, const Vector<std::string>& args)
{
  if (args.getSize() != 4 || app->graphs_.has(args[1])) throwInvalid();
  auto* cell1 = app->graphs_.find(args[2]);
  auto* cell2 = app->graphs_.find(args[3]);
  if (!cell1 || !cell2) throwInvalid();
  Graph merged = cell1->value.merge(cell2->value);
  app->graphs_.add(args[1], merged);
}

void velizade::Application::cmdExtract(Application* app, const Vector<std::string>& args)
{
  if (args.getSize() < 4 || app->graphs_.has(args[1])) throwInvalid();
  auto* cell = app->graphs_.find(args[2]);
  unsigned long long count = 0;
  if (!cell || !tryParseUInt(args[3], count) || args.getSize() != 4 + count) throwInvalid();
  Vector<std::string> vertsToExtract;
  for (size_t i = 0; i < count; ++i)
  {
    std::string v = args[4 + i];
    for (size_t j = i + 1; j < count; ++j)
    {
      if (v == args[4 + j]) throwInvalid();
    }
    if (!cell->value.hasVertex(v)) throwInvalid();
    vertsToExtract.pushBack(v);
  }
  Graph extracted = cell->value.extract(vertsToExtract);
  app->graphs_.add(args[1], extracted);
}
