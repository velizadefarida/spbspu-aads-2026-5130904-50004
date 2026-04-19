#ifndef MATH_HPP
#define MATH_HPP

#include "stack.hpp"
#include "queue.hpp"
#include <string>

namespace velizade {

int getPriority(const std::string& op);
bool isOperator(const std::string& token);
bool isNumber(const std::string& token);
long long applyOperator(long long a, long long b, const std::string& op);
Queue<std::string> convertToPostfix(const std::string& line);
long long calculatePostfix(Queue<std::string>& postfix);
long long calculateExpression(const std::string& line);

}

#endif
