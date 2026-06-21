#include "commands.hpp"
#include "functions.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <utility>
#include <stdexcept>

velizade::Cmd::Cmd() = default;

void velizade::Cmd::loadFromFile(const std::string& filename)
{
  std::ifstream file(filename);
  if (!file.is_open())
  {
    throw std::runtime_error("Cannot open file");
  }

  std::string line;
  while (std::getline(file, line))
  {
    size_t start = line.find_first_not_of(" \t");
    if (start == std::string::npos)
    {
      continue;
    }
    line = line.substr(start);
    size_t end = line.find_last_not_of(" \t");
    if (end != std::string::npos)
    {
      line = line.substr(0, end + 1);
    }
    if (line.empty())
    {
      continue;
    }

    std::string dictName;
    std::vector<std::pair<int, std::string>> pairs;
    velizade::parseDictLine(line, dictName, pairs);

    DictTree dict;
    for (const auto& p : pairs)
    {
      dict.push(p.first, p.second);
    }
    try
    {
      datasets_.drop(dictName);
    }
    catch (...)
    {

    }
    datasets_.push(dictName, dict);
  }
}

void velizade::Cmd::processCommands(std::istream& in, std::ostream& out)
{
  std::string cmd;
  while (in >> cmd)
  {
    if (cmd == "print")
    {
      processPrint(in, out);
    }
    else if (cmd == "complement")
    {
      processComplement(in, out);
    }
    else if (cmd == "intersect")
    {
      processIntersect(in, out);
    }
    else if (cmd == "union")
    {
      processUnion(in, out);
    }
    else
    {
      out << "<INVALID COMMAND>\n";
    }
  }
}

void velizade::Cmd::processPrint(std::istream& in, std::ostream& out)
{
  std::string name;
  in >> name;
  if (!datasets_.contains(name))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto dict = datasets_.get(name);
  if (dict.empty())
  {
    out << "<EMPTY>\n";
  }
  else
  {
    out << name;
    for (auto it = dict.begin(); it != dict.end(); ++it)
    {
      out << " " << it.key() << " " << it.value();
    }
    out << "\n";
  }
}

void velizade::Cmd::processComplement(std::istream& in, std::ostream& out)
{
  std::string newName, leftName, rightName;
  in >> newName >> leftName >> rightName;
  if (!datasets_.contains(leftName) || !datasets_.contains(rightName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto leftDict = datasets_.get(leftName);
  auto rightDict = datasets_.get(rightName);

  DictTree result;
  for (auto it = leftDict.begin(); it != leftDict.end(); ++it)
  {
    if (!rightDict.contains(it.key()))
    {
      result.push(it.key(), it.value());
    }
  }

  try
  {
    datasets_.drop(newName);
  }
  catch (...)
  {

  }
  datasets_.push(newName, result);
}

void velizade::Cmd::processIntersect(std::istream& in, std::ostream& out)
{
  std::string newName, leftName, rightName;
  in >> newName >> leftName >> rightName;
  if (!datasets_.contains(leftName) || !datasets_.contains(rightName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto leftDict = datasets_.get(leftName);
  auto rightDict = datasets_.get(rightName);

  DictTree result;
  for (auto it = leftDict.begin(); it != leftDict.end(); ++it)
  {
    if (rightDict.contains(it.key()))
    {
      result.push(it.key(), it.value());
    }
  }

  try
  {
    datasets_.drop(newName);
  }
  catch (...)
  {

  }
  datasets_.push(newName, result);
}

void velizade::Cmd::processUnion(std::istream& in, std::ostream& out)
{
  std::string newName, leftName, rightName;
  in >> newName >> leftName >> rightName;
  if (!datasets_.contains(leftName) || !datasets_.contains(rightName))
  {
    out << "<INVALID COMMAND>\n";
    return;
  }
  auto leftDict = datasets_.get(leftName);
  auto rightDict = datasets_.get(rightName);

  DictTree result;
  for (auto it = leftDict.begin(); it != leftDict.end(); ++it)
  {
    result.push(it.key(), it.value());
  }
  for (auto it = rightDict.begin(); it != rightDict.end(); ++it)
  {
    if (!result.contains(it.key()))
    {
      result.push(it.key(), it.value());
    }
  }

  try
  {
    datasets_.drop(newName);
  }
  catch (...)
  {

  }
  datasets_.push(newName, result);
}
