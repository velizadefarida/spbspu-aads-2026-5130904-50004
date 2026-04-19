#include "math.hpp"
#include <cstdlib>
#include <cctype>
#include <stdexcept>

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
    return a + b;
  }
  if (op == "-") {
    return a - b;
  }
  if (op == "*") {
    return a * b;
  }
  if (op == "/") {
    if (b == 0) {
      throw std::runtime_error("Division by zero");
    }
    return a / b;
  }
  if (op == "%") {
    if (b == 0) {
      throw std::runtime_error("Modulo by zero");
    }
    return a % b;
  }
  if (op == ">>") {
    if (b < 0) {
      throw std::runtime_error("Negative shift amount");
    }
    return a >> b;
  }
  throw std::runtime_error("Unknown operator: " + op);
}

velizade::Queue<std::string> velizade::convertToPostfix(const std::string& line) {
  std::vector<std::string> tokens;
  std::string token;
  for (char ch : line) {
    if (ch == ' ') {
      if (!token.empty()) {
        tokens.push_back(token);
        token.clear();
      }
    } else {
      token.push_back(ch);
    }
  }
  if (!token.empty()) {
    tokens.push_back(token);
  }

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
      char* end;
      long long num = std::strtoll(token.c_str(), &end, 10);
      if (*end != '\0') {
        throw std::runtime_error("Invalid number");
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
