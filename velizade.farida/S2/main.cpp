#include "expression.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main(int argc, char* argv[]) {
  std::istream* input = &std::cin;
  std::ifstream file;

  if (argc >= 2) {
    file.open(argv[1]);
    if (!file.is_open()) {
      std::cerr << "Error: cannot open file " << argv[1] << "\n";
      return 1;
    }
    input = &file;
  }

  std::vector<long long> results;
  std::string line;

  while (std::getline(*input, line)) {
    if (line.empty()) {
      continue;
    }
    try {
      results.push_back(velizade::calculateExpression(line));
    } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << "\n";
      return 1;
    }
  }

  for (auto it = results.rbegin(); it != results.rend(); ++it) {
    if (it != results.rbegin()) {
      std::cout << ' ';
    }
    std::cout << *it;
  }
  std::cout << "\n";
  return 0;
}
