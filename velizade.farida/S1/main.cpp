#include "list.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <sstream>
#include <limits>

int main()
{
  try
  {
    velizade::List< std::pair< std::string, velizade::List< unsigned long long > > > sequences;
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
      velizade::List<unsigned long long> numbers;
      unsigned long long num;
      while (iss >> num)
      {
        numbers.push_front(num);
      }
      numbers.reverse();

      sequences.push_front({name, std::move(numbers)});
    }

    if (sequences.empty())
    {
      std::cout << "0\n";
      return 0;
    }

    sequences.reverse();

    auto nameIt = ++sequences.cbegin();
    std::cout << nameIt->first;
    ++nameIt;
    for (; nameIt != sequences.cend(); ++nameIt)
    {
      std::cout << " " << nameIt->first;
    }
    std::cout << "\n";

    size_t maxLen = 0;
    for (auto seq = ++sequences.cbegin(); seq != sequences.cend(); ++seq)
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

    velizade::List< velizade::List< unsigned long long > > columns;
    for (size_t i = 0; i < maxLen; ++i)
    {
      velizade::List< unsigned long long > col;
      for (auto seq = ++sequences.cbegin(); seq != sequences.cend(); ++seq)
      {
        if (i < seq->second.size())
        {
          auto elem = seq->second.cbegin();
          for (size_t j = 0; j <= i; ++j)
          {
            ++elem;
          }
          col.push_front(*elem);
        }
      }
      col.reverse();
      columns.push_front(std::move(col));
    }
    columns.reverse();

    velizade::List< unsigned long long > sums;
    bool overflow = false;

    for (auto col = ++columns.cbegin(); col != columns.cend(); ++col)
    {
      bool firstElem = true;
      unsigned long long colSum = 0;
      for (auto val = ++col->cbegin(); val != col->cend(); ++val)
      {
        if (!firstElem)
        {
          std::cout << " ";
        }
        std::cout << *val;
        firstElem = false;

        if (colSum > std::numeric_limits< unsigned long long >::max() - *val)
        {
          overflow = true;
        }
        else
        {
          colSum += *val;
        }
      }
      std::cout << "\n";
      sums.push_front(colSum);
    }
    sums.reverse();

    if (overflow)
    {
      std::cerr << "Error: overflow\n";
      return 1;
    }

    if (!sums.empty())
    {
      auto sumIt = ++sums.cbegin();
      std::cout << *sumIt;
      ++sumIt;
      for (; sumIt != sums.cend(); ++sumIt)
      {
        std::cout << " " << *sumIt;
      }
      std::cout << "\n";
    }

    return 0;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
