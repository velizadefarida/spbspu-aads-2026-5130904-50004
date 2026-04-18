#include "list.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <sstream>
#include <limits>

using namespace velizade;

int main()
{
  try
  {
    List<std::pair<std::string, List<unsigned long long>>> sequences;
    std::string line;

    while (std::getline(std::cin, line))
    {
      if (line.empty())
      {
        continue;
      }
      std::istringstream iss(line);
      std::string name;
      iss >> name;
      List<unsigned long long> numbers;
      unsigned long long num;
      while (iss >> num)
      {
        numbers.push_back(num);
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
    {
      std::cout << ' ' << nameIt->first;
    }
    std::cout << '\n';

    size_t maxLen = 0;
    for (auto seq = sequences.cbegin(); seq != sequences.cend(); ++seq)
    {
      if (seq->second.size() > maxLen)
      {
        maxLen = seq->second.size();
      }
    }
    if (maxLen == 0)
    {
      std::cout << "0\n";
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
          for (size_t j = 0; j < i; ++j)
            ++elem;
          col.push_back(*elem);
        }
      }
      columns.push_back(std::move(col));
    }

    List<unsigned long long> sums;
    bool overflow = false;
    for (auto col = columns.cbegin(); col != columns.cend(); ++col)
    {
      bool firstElem = true;
      unsigned long long colSum = 0;
      for (auto val = col->cbegin(); val != col->cend(); ++val)
      {
        if (!firstElem)
        {
          std::cout << ' ';
        }
        std::cout << *val;
        firstElem = false;
        if (colSum > std::numeric_limits<unsigned long long>::max() - *val)
        {
          overflow = true;
        }
        else
        {
          colSum += *val;
        }
      }
      std::cout << '\n';
      sums.push_back(colSum);
    }

    if (overflow)
    {
      std::cerr << "Error: overflow\n";
      return 1;
    }

    auto sumIt = sums.cbegin();
    std::cout << *sumIt;
    ++sumIt;
    for (; sumIt != sums.cend(); ++sumIt)
    {
      std::cout << ' ' << *sumIt;
    }
    std::cout << '\n';

    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << '\n';
    return 1;
  }
}
