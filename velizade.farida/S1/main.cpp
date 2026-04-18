#include "list.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <sstream>

using namespace velizade;

int main()
{
  try
  {
    List<std::pair<std::string, List<int>>> sequences;

    std::string line;
    while (std::getline(std::cin, line))
    {
      if (line.empty())
        continue;

      std::istringstream iss(line);
      std::string name;
      iss >> name;

      List<int> numbers;
      int value;
      while (iss >> value)
      {
        numbers.push_back(value);
      }
      sequences.push_back({name, std::move(numbers)});
    }

    if (sequences.empty())
    {
      std::cout << "0\n";
      return 0;
    }

    auto nameIt = sequences.cbegin();
    std::cout << nameIt->first;
    ++nameIt;
    for (; nameIt != sequences.cend(); ++nameIt)
      std::cout << ' ' << nameIt->first;
    std::cout << '\n';

    size_t maxLen = 0;
    for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
      if (it->second.size() > maxLen)
        maxLen = it->second.size();

    if (maxLen == 0)
    {
      std::cerr << "Error: cannot calculate sums\n";
      return 1;
    }

    List<List<int>> columns;
    for (size_t col = 0; col < maxLen; ++col)
    {
      List<int> column;
      for (auto seq = sequences.cbegin(); seq != sequences.cend(); ++seq)
      {
        if (col < seq->second.size())
        {
          auto elem = seq->second.cbegin();
          for (size_t k = 0; k < col; ++k)
            ++elem;
          column.push_back(*elem);
        }
      }
      columns.push_back(std::move(column));
    }

    List<long long> sums;
    for (auto col = columns.cbegin(); col != columns.cend(); ++col)
    {
      bool firstElem = true;
      long long colSum = 0;
      for (auto val = col->cbegin(); val != col->cend(); ++val)
      {
        if (!firstElem)
          std::cout << ' ';
        std::cout << *val;
        firstElem = false;
        colSum += *val;
      }
      std::cout << '\n';
      sums.push_back(colSum);
    }

    auto sumIt = sums.cbegin();
    std::cout << *sumIt;
    ++sumIt;
    for (; sumIt != sums.cend(); ++sumIt)
      std::cout << ' ' << *sumIt;
    std::cout << '\n';

    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}
