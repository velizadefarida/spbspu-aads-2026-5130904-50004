#include "commands.hpp"
#include <algorithm>
#include <stdexcept>
#include <sstream>
#include <cstdlib>

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

static unsigned long long parseUInt(const std::string& s)
{
  if (s.empty())
  {
    throw std::runtime_error("Invalid number");
  }
  unsigned long long res = 0;
  for (size_t i = 0; i < s.size(); ++i)
  {
    if (s[i] < '0' || s[i] > '9')
    {
      throw std::runtime_error("Invalid number");
    }
    res = res * 10 + (s[i] - '0');
  }
  return res;
}

struct OutEntry
{
  std::string vertex;
  velizade::Vector<unsigned long long> weights;
};

static bool cmpOutEntry(const OutEntry& a, const OutEntry& b)
{
  return a.vertex < b.vertex;
}

void velizade::cmdGraphs(const Vector<std::string>& args, std::ostream& out)
{
  if (!args.isEmpty())
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
  if (names.getSize() == 0)
  {
    out << '\n';
  }
  else
  {
    for (size_t i = 0; i < names.getSize(); ++i)
    {
      out << names[i] << '\n';
    }
  }
}

void velizade::cmdVertexes(const Vector<std::string>& args, std::ostream& out)
{
  if (args.getSize() != 1)
  {
    throw std::runtime_error("Invalid arguments");
  }

  const auto& g = getGraphCheckedConst(args[0]);
  Vector<std::string> vlist = g.vertices;
  std::sort(vlist.begin(), vlist.end());
  if (vlist.getSize() == 0)
  {
    out << '\n';
  }
  else
  {
    for (size_t i = 0; i < vlist.getSize(); ++i)
    {
      out << vlist[i] << '\n';
    }
  }
}

void velizade::cmdOutbound(const Vector<std::string>& args, std::ostream& out)
{
  if (args.getSize() != 2)
  {
    throw std::runtime_error("Invalid arguments");
  }

  const auto& g = getGraphCheckedConst(args[0]);
  const std::string& vertex = args[1];
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
  if (entries.getSize() == 0)
  {
    out << '\n';
  }
  else
  {
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
}

void velizade::cmdInbound(const Vector<std::string>& args, std::ostream& out)
{
  if (args.getSize() != 2)
  {
    throw std::runtime_error("Invalid arguments");
  }

  const auto& g = getGraphCheckedConst(args[0]);
  const std::string& vertex = args[1];
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
  if (entries.getSize() == 0)
  {
    out << '\n';
  }
  else
  {
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
}

void velizade::cmdBind(const Vector<std::string>& args, std::ostream& out)
{
  (void)out;
  if (args.getSize() != 4)
  {
    throw std::runtime_error("Invalid arguments");
  }

  std::string graphName = args[0];
  std::string from = args[1];
  std::string to = args[2];
  unsigned long long weight = parseUInt(args[3]);

  auto& g = getGraphChecked(graphName);
  g.addVertex(from);
  g.addVertex(to);
  g.addEdge(from, to, weight);
}

void velizade::cmdCut(const Vector<std::string>& args, std::ostream& out)
{
  (void)out;
  if (args.getSize() != 4)
  {
    throw std::runtime_error("Invalid arguments");
  }

  std::string graphName = args[0];
  std::string from = args[1];
  std::string to = args[2];
  unsigned long long weight = parseUInt(args[3]);

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

void velizade::cmdCreate(const Vector<std::string>& args, std::ostream& out)
{
  (void)out;
  if (args.getSize() < 2)
  {
    throw std::runtime_error("Invalid arguments");
  }

  std::string graphName = args[0];
  size_t count = parseUInt(args[1]);
  if (args.getSize() != 2 + count)
  {
    throw std::runtime_error("Invalid arguments");
  }

  auto& graphs = getGraphs();
  if (graphs.has(graphName))
  {
    throw std::runtime_error("Graph already exists");
  }

  Graph newGraph(graphName);
  for (size_t i = 0; i < count; ++i)
  {
    newGraph.addVertex(args[2 + i]);
  }
  graphs.add(graphName, std::move(newGraph));
}

void velizade::cmdMerge(const Vector<std::string>& args, std::ostream& out)
{
  (void)out;
  if (args.getSize() != 3)
  {
    throw std::runtime_error("Invalid arguments");
  }

  std::string newName = args[0];
  std::string g1 = args[1];
  std::string g2 = args[2];

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

void velizade::cmdExtract(const Vector<std::string>& args, std::ostream& out)
{
  (void)out;
  if (args.getSize() < 3)
  {
    throw std::runtime_error("Invalid arguments");
  }

  std::string newName = args[0];
  std::string oldName = args[1];
  size_t count = parseUInt(args[2]);
  if (args.getSize() != 3 + count)
  {
    throw std::runtime_error("Invalid arguments");
  }

  auto& graphs = getGraphs();
  if (graphs.has(newName))
  {
    throw std::runtime_error("Graph already exists");
  }
  const auto& old = getGraphCheckedConst(oldName);

  Vector<std::string> selected;
  for (size_t i = 0; i < count; ++i)
  {
    std::string v = args[3 + i];
    if (!old.hasVertex(v))
    {
      throw std::runtime_error("Vertex not found in source");
    }
    selected.pushBack(v);
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

bool velizade::CommandManager::cmd(const std::string& name, const Vector<std::string>& args, std::ostream& out) const
{
  auto* cell = cmds_.find(name);
  if (!cell)
  {
    return false;
  }
  try
  {
    cell->value(args, out);
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
