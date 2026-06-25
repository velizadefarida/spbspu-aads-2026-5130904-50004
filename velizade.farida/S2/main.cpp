#include "expression.hpp"
#include "stack.hpp"
#include <iostream>
#include <fstream>
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

  velizade::Stack<long long> results;
  std::string line;

  while (std::getline(*input, line)) {
    if (line.empty()) {
      continue;
    }
    try {
      results.push(velizade::calculateExpression(line));
    } catch (const std::exception& e) {
      std::cerr << "Error: " << e.what() << "\n";
      return 1;
    }
  }

  bool first = true;
  while (!results.empty()) {
    if (!first) {
      std::cout << ' ';
    }
    first = false;
    std::cout << results.top();
    results.pop();
  }
  std::cout << "\n";
  return 0;
}
