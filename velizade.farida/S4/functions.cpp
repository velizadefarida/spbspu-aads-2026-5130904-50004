#include "functions.hpp"
#include <string>
#include <vector>
#include <utility>

std::vector<std::string> velizade::split(const std::string& str, char delim)
{
  std::vector<std::string> tokens;
  size_t start = 0;
  size_t pos = str.find(delim);
  while (pos != std::string::npos)
  {
    tokens.push_back(str.substr(start, pos - start));
    start = pos + 1;
    pos = str.find(delim, start);
  }
  if (start < str.size())
  {
    tokens.push_back(str.substr(start));
  }
  return tokens;
}

void velizade::parseDictLine(const std::string& line, std::string& dictName, std::vector<std::pair<int, std::string>>& pairs)
{
  auto tokens = split(line, ' ');
  if (tokens.empty())
  {
    return;
  }
  dictName = tokens[0];
  for (size_t i = 1; i + 1 < tokens.size(); i += 2)
  {
    int key = std::stoi(tokens[i]);
    std::string value = tokens[i + 1];
    pairs.emplace_back(key, value);
  }
}
