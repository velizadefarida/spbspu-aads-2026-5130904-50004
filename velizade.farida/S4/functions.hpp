#ifndef FUNCTIONS_HPP
#define FUNCTIONS_HPP

#include <string>
#include <vector>
#include <utility>

namespace velizade
{
  std::vector<std::string> split(const std::string& str, char delim);

  void parseDictLine(const std::string& line, std::string& dictName, std::vector<std::pair<int, std::string>>& pairs);
}

#endif
