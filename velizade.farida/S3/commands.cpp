#include "commands.hpp"
#include <algorithm>
#include <stdexcept>
#include <sstream>

static velizade::Graph& getGraphChecked(const std::string& name)
{
  auto& graphs = velizade::getGraphs();
  auto* cell = graphs.find(name);
  if (!cell)
  {
    throw std::runtime_error("Graph not found");
  }
  return cell->value;
}

static const velizade::Graph& getGraphCheckedConst(const std::string& name)
{
  auto& graphs = velizade::getGraphs();
  const auto* cell = graphs.find(name);
  if (!cell)
  {
    throw std::runtime_error("Graph not found");
  }
  return cell->value;
}

struct OutEntry
{
  std::string vertex;
  velizade::Vector<int> weights;
};

static bool cmpOutEntry(const OutEntry& a, const OutEntry& b)
{
  return a.vertex < b.vertex;
}

void velizade::cmdGraphs(std::istream& in, std::ostream& out)
{
  std::string dummy;
  if (in >> dummy)
  {
    throw std::runtime_error("Invalid arguments");
  }

  auto& graphs = getGraphs();
  Vector<std::string> names;
  for (auto it = graphs.begin(); it != graphs.end(); ++it)
  {
    auto kv = *it;
    names.pushBack(kv.first);
  }
  std::sort(names.begin(), names.end());
  for (size_t i = 0; i < names.getSize(); ++i)
  {
    out << names[i] << '\n';
  }
}

void velizade::cmdVertexes(std::istream& in, std::ostream& out)
{
  std::string graphName;
  if (!(in >> graphName))
  {
    throw std::runtime_error("Invalid arguments");
  }
  std::string dummy;
  if (in >> dummy)
  {
    throw std::runtime_error("Invalid arguments");
  }

  const auto& g = getGraphCheckedConst(graphName);
  Vector<std::string> vlist = g.vertices;
  std::sort(vlist.begin(), vlist.end());
  for (size_t i = 0; i < vlist.getSize(); ++i)
  {
    out << vlist[i] << '\n';
  }
}

void velizade::cmdOutbound(std::istream& in, std::ostream& out)
{
  std::string graphName, vertex;
  if (!(in >> graphName >> vertex))
  {
    throw std::runtime_error("Invalid arguments");
  }
  std::string dummy;
  if (in >> dummy)
  {
    throw std::runtime_error("Invalid arguments");
  }

  const auto& g = getGraphCheckedConst(graphName);
  if (!g.hasVertex(vertex))
  {
    throw std::runtime_error("Vertex not found");
  }

  Vector<OutEntry> entries;
  for (auto it = g.edges.begin(); it != g.edges.end(); ++it)
  {
    auto kv = *it;
    if (kv.first.first == vertex)
    {
      OutEntry e;
      e.vertex = kv.first.second;
      e.weights = kv.second;
      std::sort(e.weights.begin(), e.weights.end());
      entries.pushBack(e);
    }
  }
  std::sort(entries.begin(), entries.end(), cmpOutEntry);
  for (size_t i = 0; i < entries.getSize(); ++i)
  {
    out << entries[i].vertex;
    for (size_t j = 0; j < entries[i].weights.getSize(); ++j)
    {
      out << ' ' << entries[i].weights[j];
    }
    out << '\n';
  }
}

void velizade::cmdInbound(std::istream& in, std::ostream& out)
{
  std::string graphName, vertex;
  if (!(in >> graphName >> vertex))
  {
    throw std::runtime_error("Invalid arguments");
  }
  std::string dummy;
  if (in >> dummy)
  {
    throw std::runtime_error("Invalid arguments");
  }

  const auto& g = getGraphCheckedConst(graphName);
  if (!g.hasVertex(vertex))
  {
    throw std::runtime_error("Vertex not found");
  }

  Vector<OutEntry> entries;
  for (auto it = g.edges.begin(); it != g.edges.end(); ++it)
  {
    auto kv = *it;
    if (kv.first.second == vertex)
    {
      OutEntry e;
      e.vertex = kv.first.first;
      e.weights = kv.second;
      std::sort(e.weights.begin(), e.weights.end());
      entries.pushBack(e);
    }
  }
  std::sort(entries.begin(), entries.end(), cmpOutEntry);
  for (size_t i = 0; i < entries.getSize(); ++i)
  {
    out << entries[i].vertex;
    for (size_t j = 0; j < entries[i].weights.getSize(); ++j)
    {
      out << ' ' << entries[i].weights[j];
    }
    out << '\n';
  }
}

void velizade::cmdBind(std::istream& in, std::ostream& out)
{
  (void)out;
  std::string graphName, from, to;
  int weight;
  if (!(in >> graphName >> from >> to >> weight))
  {
    throw std::runtime_error("Invalid arguments");
  }
  std::string dummy;
  if (in >> dummy)
  {
    throw std::runtime_error("Invalid arguments");
  }

  auto& g = getGraphChecked(graphName);
  g.addVertex(from);
  g.addVertex(to);
  g.addEdge(from, to, weight);
}

void velizade::cmdCut(std::istream& in, std::ostream& out)
{
  (void)out;
  std::string graphName, from, to;
  int weight;
  if (!(in >> graphName >> from >> to >> weight))
  {
    throw std::runtime_error("Invalid arguments");
  }
  std::string dummy;
  if (in >> dummy)
  {
    throw std::runtime_error("Invalid arguments");
  }

  auto& g = getGraphChecked(graphName);
  if (!g.hasVertex(from) || !g.hasVertex(to))
  {
    throw std::runtime_error("Vertex not found");
  }
  if (!g.removeEdge(from, to, weight))
  {
    throw std::runtime_error("Edge not found");
  }
}

void velizade::cmdCreate(std::istream& in, std::ostream& out)
{
  (void)out;
  std::string graphName;
  size_t count;
  if (!(in >> graphName >> count))
  {
    throw std::runtime_error("Invalid arguments");
  }
  Vector<std::string> vertices;
  for (size_t i = 0; i < count; ++i)
  {
    std::string v;
    if (!(in >> v))
    {
      throw std::runtime_error("Invalid arguments");
    }
    vertices.pushBack(v);
  }
  std::string dummy;
  if (in >> dummy)
  {
    throw std::runtime_error("Invalid arguments");
  }

  auto& graphs = getGraphs();
  if (graphs.has(graphName))
  {
    throw std::runtime_error("Graph already exists");
  }
  Graph newGraph(graphName);
  for (size_t i = 0; i < vertices.getSize(); ++i)
  {
    newGraph.addVertex(vertices[i]);
  }
  graphs.add(graphName, std::move(newGraph));
}

void velizade::cmdMerge(std::istream& in, std::ostream& out)
{
  (void)out;
  std::string newName, g1, g2;
  if (!(in >> newName >> g1 >> g2))
  {
    throw std::runtime_error("Invalid arguments");
  }
  std::string dummy;
  if (in >> dummy)
  {
    throw std::runtime_error("Invalid arguments");
  }

  auto& graphs = getGraphs();
  if (graphs.has(newName))
  {
    throw std::runtime_error("Graph already exists");
  }
  const auto& gr1 = getGraphCheckedConst(g1);
  const auto& gr2 = getGraphCheckedConst(g2);

  Graph merged(newName);
  for (size_t i = 0; i < gr1.vertices.getSize(); ++i)
  {
    merged.addVertex(gr1.vertices[i]);
  }
  for (size_t i = 0; i < gr2.vertices.getSize(); ++i)
  {
    merged.addVertex(gr2.vertices[i]);
  }

  for (auto it = gr1.edges.begin(); it != gr1.edges.end(); ++it)
  {
    auto kv = *it;
    const auto& weights = kv.second;
    for (size_t i = 0; i < weights.getSize(); ++i)
    {
      merged.addEdge(kv.first.first, kv.first.second, weights[i]);
    }
  }
  for (auto it = gr2.edges.begin(); it != gr2.edges.end(); ++it)
  {
    auto kv = *it;
    const auto& weights = kv.second;
    for (size_t i = 0; i < weights.getSize(); ++i)
    {
      merged.addEdge(kv.first.first, kv.first.second, weights[i]);
    }
  }
  graphs.add(newName, std::move(merged));
}

void velizade::cmdExtract(std::istream& in, std::ostream& out)
{
  (void)out;
  std::string newName, oldName;
  size_t count;
  if (!(in >> newName >> oldName >> count))
  {
    throw std::runtime_error("Invalid arguments");
  }
  Vector<std::string> selected;
  for (size_t i = 0; i < count; ++i)
  {
    std::string v;
    if (!(in >> v))
    {
      throw std::runtime_error("Invalid arguments");
    }
    selected.pushBack(v);
  }
  std::string dummy;
  if (in >> dummy)
  {
    throw std::runtime_error("Invalid arguments");
  }

  auto& graphs = getGraphs();
  if (graphs.has(newName))
  {
    throw std::runtime_error("Graph already exists");
  }
  const auto& old = getGraphCheckedConst(oldName);

  for (size_t i = 0; i < selected.getSize(); ++i)
  {
    if (!old.hasVertex(selected[i]))
    {
      throw std::runtime_error("Vertex not found in source");
    }
  }
  for (size_t i = 0; i < selected.getSize(); ++i)
  {
    for (size_t j = i + 1; j < selected.getSize(); ++j)
    {
      if (selected[i] == selected[j])
      {
        throw std::runtime_error("Duplicate vertices");
      }
    }
  }

  Graph extracted(newName);
  for (size_t i = 0; i < selected.getSize(); ++i)
  {
    extracted.addVertex(selected[i]);
  }

  for (auto it = old.edges.begin(); it != old.edges.end(); ++it)
  {
    auto kv = *it;
    const std::string& from = kv.first.first;
    const std::string& to = kv.first.second;
    bool hasFrom = false, hasTo = false;
    for (size_t i = 0; i < selected.getSize(); ++i)
    {
      if (selected[i] == from)
      {
        hasFrom = true;
      }
      if (selected[i] == to)
      {
        hasTo = true;
      }
      if (hasFrom && hasTo)
      {
        break;
      }
    }
    if (hasFrom && hasTo)
    {
      const auto& weights = kv.second;
      for (size_t i = 0; i < weights.getSize(); ++i)
      {
        extracted.addEdge(from, to, weights[i]);
      }
    }
  }
  graphs.add(newName, std::move(extracted));
}

velizade::CommandManager::CommandManager() :
  cmds_(32, 4)
{
  cmds_.add("graphs", cmdGraphs);
  cmds_.add("vertexes", cmdVertexes);
  cmds_.add("outbound", cmdOutbound);
  cmds_.add("inbound", cmdInbound);
  cmds_.add("bind", cmdBind);
  cmds_.add("cut", cmdCut);
  cmds_.add("create", cmdCreate);
  cmds_.add("merge", cmdMerge);
  cmds_.add("extract", cmdExtract);
}

bool velizade::CommandManager::cmd(const std::string& name, std::istream& in, std::ostream& out) const
{
  auto* cell = cmds_.find(name);
  if (!cell)
  {
    return false;
  }
  try
  {
    cell->value(in, out);
  }
  catch (const std::exception&)
  {
    out << "<INVALID COMMAND>\n";
  }
  return true;
}

void velizade::CommandManager::addCommand(const std::string& name, func_t cmd)
{
  cmds_.add(name, cmd);
}
