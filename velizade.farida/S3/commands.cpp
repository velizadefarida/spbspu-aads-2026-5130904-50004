#include "commands.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>

velizade::Vector<std::string> velizade::splitString(const std::string& str)
{
  Vector<std::string> res;
  std::string cur;
  for (char ch : str)
  {
    if (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
    {
      if (!cur.empty())
      {
        res.pushBack(cur);
        cur.clear();
      }
    }
    else
    {
      cur += ch;
    }
  }
  if (!cur.empty())
  {
    res.pushBack(cur);
  }
  return res;
}

bool velizade::tryParseUInt(const std::string& str, unsigned int& out)
{
  if (str.empty())
  {
    return false;
  }
  unsigned long long val = 0;
  for (char ch : str)
  {
    if (ch < '0' || ch > '9')
    {
      return false;
    }
    val = val * 10 + (ch - '0');
    if (val > 0xFFFFFFFFULL)
    {
      return false;
    }
  }
  out = static_cast<unsigned int>(val);
  return true;
}

void velizade::throwInvalid()
{
  throw std::invalid_argument("INVALID COMMAND");
}

velizade::Application::Application():
    commands_(16, 4, 4),
    graphs_(16, 4, 4)
{
  commands_.add("graphs", &cmdGraphs);
  commands_.add("vertexes", &cmdVertexes);
  commands_.add("outbound", &cmdOutbound);
  commands_.add("inbound", &cmdInbound);
  commands_.add("bind", &cmdBind);
  commands_.add("cut", &cmdCut);
  commands_.add("create", &cmdCreate);
  commands_.add("merge", &cmdMerge);
  commands_.add("extract", &cmdExtract);
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
    if (tokens.isEmpty())
    {
      continue;
    }

    if (tokens.getSize() == 2)
    {
      std::string graphName = tokens[0];
      unsigned int edgeCount;
      if (!tryParseUInt(tokens[1], edgeCount))
      {
        continue;
      }

      Graph g;
      for (unsigned int i = 0; i < edgeCount; ++i)
      {
        if (!std::getline(file, line))
        {
          break;
        }
        Vector<std::string> et = splitString(line);
        if (et.getSize() >= 3)
        {
          unsigned int w;
          if (tryParseUInt(et[2], w))
          {
            g.addEdge(et[0], et[1], w);
          }
        }
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
  if (tokens.isEmpty())
  {
    return;
  }

  auto it = commands_.find(tokens[0]);
  if (it != commands_.end())
  {
    try
    {
      (it->second)(this, tokens);
    }
    catch (const std::invalid_argument&)
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
  if (args.getSize() != 1)
  {
    throwInvalid();
  }

  Vector<std::string> names;
  for (auto it = app->graphs_.cbegin(); it != app->graphs_.cend(); ++it)
  {
    names.pushBack(it->first);
  }

  sortVector(names);
  if (names.isEmpty())
  {
    std::cout << "\n";
    return;
  }
  for (size_t i = 0; i < names.getSize(); ++i)
  {
    std::cout << names[i] << "\n";
  }
}

void velizade::Application::cmdVertexes(Application* app, const Vector<std::string>& args)
{
  if (args.getSize() != 2)
  {
    throwInvalid();
  }

  auto it = app->graphs_.find(args[1]);
  if (it == app->graphs_.end())
  {
    throwInvalid();
  }

  Vector<std::string> verts = it->second.getVertexes();
  sortVector(verts);
  if (verts.isEmpty())
  {
    std::cout << "\n";
    return;
  }
  for (size_t i = 0; i < verts.getSize(); ++i)
  {
    std::cout << verts[i] << "\n";
  }
}

void velizade::Application::cmdOutbound(Application* app, const Vector<std::string>& args)
{
  if (args.getSize() != 3)
  {
    throwInvalid();
  }

  auto it = app->graphs_.find(args[1]);
  if (it == app->graphs_.end() || !it->second.hasVertex(args[2]))
  {
    throwInvalid();
  }

  Vector<std::pair<std::string, unsigned int>> res;
  it->second.getOutbound(args[2], res);

  if (res.isEmpty())
  {
    std::cout << "\n";
    return;
  }

  std::string curV = res[0].first;
  std::cout << curV << " " << res[0].second;
  for (size_t i = 1; i < res.getSize(); ++i)
  {
    if (res[i].first == curV)
    {
      std::cout << " " << res[i].second;
    }
    else
    {
      curV = res[i].first;
      std::cout << "\n" << curV << " " << res[i].second;
    }
  }
  std::cout << "\n";
}

void velizade::Application::cmdInbound(Application* app, const Vector<std::string>& args)
{
  if (args.getSize() != 3)
  {
    throwInvalid();
  }

  auto it = app->graphs_.find(args[1]);
  if (it == app->graphs_.end() || !it->second.hasVertex(args[2]))
  {
    throwInvalid();
  }

  Vector<std::pair<std::string, unsigned int>> res;
  it->second.getInbound(args[2], res);

  if (res.isEmpty())
  {
    std::cout << "\n";
    return;
  }

  std::string curV = res[0].first;
  std::cout << curV << " " << res[0].second;
  for (size_t i = 1; i < res.getSize(); ++i)
  {
    if (res[i].first == curV)
    {
      std::cout << " " << res[i].second;
    }
    else
    {
      curV = res[i].first;
      std::cout << "\n" << curV << " " << res[i].second;
    }
  }
  std::cout << "\n";
}

void velizade::Application::cmdBind(Application* app, const Vector<std::string>& args)
{
  unsigned int weight;
  if (args.getSize() != 5 || !tryParseUInt(args[4], weight))
  {
    throwInvalid();
  }

  auto it = app->graphs_.find(args[1]);
  if (it == app->graphs_.end())
  {
    throwInvalid();
  }

  it->second.addEdge(args[2], args[3], weight);
}

void velizade::Application::cmdCut(Application* app, const Vector<std::string>& args)
{
  unsigned int weight;
  if (args.getSize() != 5 || !tryParseUInt(args[4], weight))
  {
    throwInvalid();
  }

  auto it = app->graphs_.find(args[1]);
  if (it == app->graphs_.end() || !it->second.cutEdge(args[2], args[3], weight))
  {
    throwInvalid();
  }
}

void velizade::Application::cmdCreate(Application* app, const Vector<std::string>& args)
{
  unsigned int count;
  if (args.getSize() < 3 || !tryParseUInt(args[2], count) || args.getSize() != 3 + count)
  {
    throwInvalid();
  }

  if (app->graphs_.has(args[1]))
  {
    throwInvalid();
  }

  for (unsigned int i = 0; i < count; ++i)
  {
    for (unsigned int j = i + 1; j < count; ++j)
    {
      if (args[3 + i] == args[3 + j])
      {
        throwInvalid();
      }
    }
  }

  Graph g;
  for (unsigned int i = 0; i < count; ++i)
  {
    g.addVertex(args[3 + i]);
  }

  app->graphs_.add(args[1], g);
}

void velizade::Application::cmdMerge(Application* app, const Vector<std::string>& args)
{
  if (args.getSize() != 4 || app->graphs_.has(args[1]))
  {
    throwInvalid();
  }

  auto it1 = app->graphs_.find(args[2]);
  auto it2 = app->graphs_.find(args[3]);
  if (it1 == app->graphs_.end() || it2 == app->graphs_.end())
  {
    throwInvalid();
  }

  app->graphs_.add(args[1], it1->second.merge(it2->second));
}

void velizade::Application::cmdExtract(Application* app, const Vector<std::string>& args)
{
  if (args.getSize() < 4 || app->graphs_.has(args[1]))
  {
    throwInvalid();
  }

  unsigned int count;
  auto it = app->graphs_.find(args[2]);
  if (it == app->graphs_.end() || !tryParseUInt(args[3], count) || args.getSize() != 4 + count)
  {
    throwInvalid();
  }

  Vector<std::string> verts;
  for (unsigned int i = 0; i < count; ++i)
  {
    std::string v = args[4 + i];
    for (unsigned int j = i + 1; j < count; ++j)
    {
      if (v == args[4 + j])
      {
        throwInvalid();
      }
    }
    if (!it->second.hasVertex(v))
    {
      throwInvalid();
    }
    verts.pushBack(v);
  }

  app->graphs_.add(args[1], it->second.extract(verts));
}
