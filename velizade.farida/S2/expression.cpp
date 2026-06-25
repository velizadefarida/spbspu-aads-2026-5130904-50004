#include "expression.hpp"
#include <limits>
#include <cctype>
#include <stdexcept>
#include <vector>

static bool willAdditionOverflow(long long a, long long b) {
  if (b > 0 && a > std::numeric_limits<long long>::max() - b) {
    return true;
  }
  if (b < 0 && a < std::numeric_limits<long long>::min() - b) {
    return true;
  }
  return false;
}

static bool willSubtractionOverflow(long long a, long long b) {
  if (b > 0 && a < std::numeric_limits<long long>::min() + b) {
    return true;
  }
  if (b < 0 && a > std::numeric_limits<long long>::max() + b) {
    return true;
  }
  return false;
}

static bool willMultiplicationOverflow(long long a, long long b) {
  if (a == 0 || b == 0) {
    return false;
  }
  long long max_val = std::numeric_limits<long long>::max();
  long long min_val = std::numeric_limits<long long>::min();
  if (a > 0 && b > 0 && a > max_val / b) {
    return true;
  }
  if (a > 0 && b < 0 && b < min_val / a) {
    return true;
  }
  if (a < 0 && b > 0 && a < min_val / b) {
    return true;
  }
  if (a < 0 && b < 0 && a < max_val / b) {
    return true;
  }
  return false;
}

static long long positiveMod(long long a, long long b) {
  long long r = a % b;
  if (r < 0) {
    r += (b > 0 ? b : -b);
  }
  return r;
}

int velizade::getPriority(const std::string& op) {
  if (op == ">>") {
    return 1;
  }
  if (op == "+" || op == "-") {
    return 2;
  }
  if (op == "*" || op == "/" || op == "%") {
    return 3;
  }
  return 0;
}

bool velizade::isOperator(const std::string& token) {
  return token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == ">>";
}

bool velizade::isNumber(const std::string& token) {
  if (token.empty()) {
    return false;
  }
  char* end;
  std::strtoll(token.c_str(), &end, 10);
  return *end == '\0';
}

long long velizade::applyOperator(long long a, long long b, const std::string& op) {
  if (op == "+") {
    if (willAdditionOverflow(a, b)) {
      throw std::runtime_error("Addition overflow");
    }
    return a + b;
  }
  if (op == "-") {
    if (willSubtractionOverflow(a, b)) {
      throw std::runtime_error("Subtraction overflow");
    }
    return a - b;
  }
  if (op == "*") {
    if (willMultiplicationOverflow(a, b)) {
      throw std::runtime_error("Multiplication overflow");
    }
    return a * b;
  }
  if (op == "/") {
    if (b == 0) {
      throw std::runtime_error("Division by zero");
    }
    if (a == std::numeric_limits<long long>::min() && b == -1) {
      throw std::runtime_error("Division overflow");
    }
    return a / b;
  }
  if (op == "%") {
    if (b == 0) {
      throw std::runtime_error("Modulo by zero");
    }
    return positiveMod(a, b);
  }
  if (op == ">>") {
    if (b < 0) {
      throw std::runtime_error("Negative shift amount");
    }
    return a >> b;
  }
  throw std::runtime_error("Unknown operator: " + op);
}

static std::vector<std::string> tokenize(const std::string& line) {
  std::vector<std::string> tokens;
  size_t i = 0;
  while (i < line.size()) {
    if (std::isspace(line[i])) {
      ++i;
      continue;
    }
    if (line[i] == '(' || line[i] == ')') {
      tokens.push_back(std::string(1, line[i]));
      ++i;
      continue;
    }
    if (line[i] == '>' && i + 1 < line.size() && line[i+1] == '>') {
      tokens.push_back(">>");
      i += 2;
      continue;
    }
    if (line[i] == '+' || line[i] == '-' || line[i] == '*' || line[i] == '/' || line[i] == '%') {
      tokens.push_back(std::string(1, line[i]));
      ++i;
      continue;
    }
    if (std::isdigit(line[i]) || (line[i] == '-' && i + 1 < line.size() && std::isdigit(line[i+1]))) {
      size_t start = i;
      if (line[i] == '-') {
        ++i;
      }
      while (i < line.size() && std::isdigit(line[i])) {
        ++i;
      }
      tokens.push_back(line.substr(start, i - start));
      continue;
    }
    throw std::runtime_error("Invalid character in expression");
  }
  return tokens;
}

velizade::Queue<std::string> velizade::convertToPostfix(const std::string& line) {
  std::vector<std::string> tokens = tokenize(line);

  velizade::Queue<std::string> output;
  velizade::Stack<std::string> ops;

  for (const std::string& tok : tokens) {
    if (velizade::isNumber(tok)) {
      output.push(tok);
    } else if (tok == "(") {
      ops.push(tok);
    } else if (tok == ")") {
      while (!ops.empty() && ops.top() != "(") {
        output.push(ops.pop());
      }
      if (ops.empty()) {
        throw std::runtime_error("Mismatched parentheses");
      }
      ops.pop();
    } else if (velizade::isOperator(tok)) {
      while (!ops.empty() && ops.top() != "(" && velizade::getPriority(ops.top()) >= velizade::getPriority(tok)) {
        output.push(ops.pop());
      }
      ops.push(tok);
    } else {
      throw std::runtime_error("Invalid token: " + tok);
    }
  }

  while (!ops.empty()) {
    if (ops.top() == "(") {
      throw std::runtime_error("Mismatched parentheses");
    }
    output.push(ops.pop());
  }
  return output;
}

long long velizade::calculatePostfix(velizade::Queue<std::string>& postfix) {
  velizade::Stack<long long> values;

  while (!postfix.empty()) {
    std::string token = postfix.pop();
    if (velizade::isNumber(token)) {
      long long num;
      try {
        num = std::stoll(token);
      } catch (const std::out_of_range&) {
        throw std::runtime_error("Invalid number: " + token);
      }
      values.push(num);
    } else if (velizade::isOperator(token)) {
      if (values.size() < 2) {
        throw std::runtime_error("Not enough operands");
      }
      long long b = values.pop();
      long long a = values.pop();
      long long res = velizade::applyOperator(a, b, token);
      values.push(res);
    } else {
      throw std::runtime_error("Invalid token in postfix: " + token);
    }
  }

  if (values.size() != 1) {
    throw std::runtime_error("Invalid expression: leftover operands");
  }
  return values.pop();
}

long long velizade::calculateExpression(const std::string& line) {
  if (line.empty()) {
    throw std::runtime_error("Empty expression");
  }
  velizade::Queue<std::string> postfix = velizade::convertToPostfix(line);
  return velizade::calculatePostfix(postfix);
}
