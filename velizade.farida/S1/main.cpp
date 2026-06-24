#include "list.hpp"
#include <iostream>
#include <string>
#include <utility>
#include <limits>
#include <stdexcept>

int main()
{
  try
  {
    velizade::List< std::pair< std::string, velizade::List< unsigned long long > > > sequences;
    std::string name;

    while (std::cin >> name)
    {
      velizade::List< unsigned long long > numbers;
      unsigned long long num;
      while (std::cin >> num)
      {
        numbers.push_front(num);
      }
      numbers.reverse();
      sequences.push_front({name, std::move(numbers)});
      std::cin.clear();
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

    for (auto col = ++columns.cbegin(); col != columns.cend(); ++col)
    {
      bool first = true;
      for (auto val = ++col->cbegin(); val != col->cend(); ++val)
      {
        if (!first)
        {
          std::cout << ' ';
        }
        std::cout << *val;
        first = false;
      }
      std::cout << "\n";
    }

    velizade::List< unsigned long long > sums;
    for (auto col = ++columns.cbegin(); col != columns.cend(); ++col)
    {
      unsigned long long colSum = 0;
      for (auto val = ++col->cbegin(); val != col->cend(); ++val)
      {
        if (colSum > std::numeric_limits< unsigned long long >::max() - *val)
        {
          throw std::overflow_error("overflow");
        }
        colSum += *val;
      }
      sums.push_front(colSum);
    }
    sums.reverse();

    if (!sums.empty())
    {
      auto it = ++sums.cbegin();
      if (it != sums.cend())
      {
        std::cout << *it;
        ++it;
        for (; it != sums.cend(); ++it)
        {
          std::cout << " " << *it;
        }
        std::cout << "\n";
      }
    }

    return 0;
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
  catch (const std::exception& e)
  {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }
}
