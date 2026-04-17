#include "list.hpp"
#include <iostream>
#include <string>
#include <cctype>
#include <limits>

using namespace velizade;

int main()
{
  try
  {
    List<std::pair<std::string, List<unsigned long long>>> sequences;
    std::string name;

    while (std::cin >> name)
    {
      List<unsigned long long> nums;
      while (std::cin.peek() == ' ' || std::cin.peek() == '\t') {
        std::cin.get();
      }

      while (std::cin.peek() != EOF && std::cin.peek() != '\n')
      {
        if (std::isdigit(std::cin.peek()))
        {
          unsigned long long val;
          std::cin >> val;
          nums.push_front(val);
          while (std::cin.peek() == ' ' || std::cin.peek() == '\t') {
            std::cin.get();
          }
        }
        else {
          break;
        }
      }
      if (std::cin.peek() == '\n') {
        std::cin.get();
      }

      List<unsigned long long> reversed;
      for (auto it = nums.cbegin(); it != nums.cend(); ++it) {
        reversed.push_front(*it);
      }
      sequences.push_front({name, std::move(reversed)});
    }

    if (sequences.empty())
    {
      std::cout << "0" << "\n";
      return 0;
    }

    List<std::pair<std::string, List<unsigned long long>>> ordered;
    for (auto it = sequences.cbegin(); it != sequences.cend(); ++it) {
      ordered.push_front(*it);
    }
    sequences = std::move(ordered);

    bool first = true;
    for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
    {
      if (!first) {
        std::cout << ' ';
      }
      std::cout << it->first;
      first = false;
    }
    std::cout << "\n";

    size_t maxLen = 0;
    for (auto it = sequences.cbegin(); it != sequences.cend(); ++it) {
      if (it->second.size() > maxLen) {
        maxLen = it->second.size();
      }
    }
    if (maxLen == 0)
    {
      return 0;
    }

    List<List<unsigned long long>> columns;
    for (size_t i = 0; i < maxLen; ++i)
    {
      List<unsigned long long> col;
      for (auto seq = sequences.cbegin(); seq != sequences.cend(); ++seq)
      {
        if (i < seq->second.size())
        {
          auto elem = seq->second.cbegin();
          for (size_t j = 0; j < i; ++j) {
            ++elem;
          }
          col.push_back(*elem);
        }
      }
      columns.push_back(std::move(col));
    }

    List<unsigned long long> sums;
    for (auto col = columns.cbegin(); col != columns.cend(); ++col)
    {
      bool firstInRow = true;
      unsigned long long sum = 0;
      for (auto elem = col->cbegin(); elem != col->cend(); ++elem)
      {
        if (!firstInRow) {
          std::cout << ' ';
        }
        std::cout << *elem;
        firstInRow = false;
        if (*elem > std::numeric_limits<unsigned long long>::max() - sum) {
          throw std::overflow_error("overflow");
        }
        sum += *elem;
      }
      std::cout << "\n";
      sums.push_back(sum);
    }

    first = true;
    for (auto it = sums.cbegin(); it != sums.cend(); ++it)
    {
      if (!first) {
        std::cout << ' ';
      }
      std::cout << *it;
      first = false;
    }
    std::cout << "\n";

    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
