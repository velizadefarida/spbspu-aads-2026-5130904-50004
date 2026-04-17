#include "list.hpp"
#include <iostream>
#include <string>
#include <cctype>
#include <limits>
#include <stdexcept>

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
      while (std::cin.peek() == ' ' || std::cin.peek() == '\t')
      {
        std::cin.get();
      }
      while (std::cin.peek() != EOF && std::cin.peek() != '\n')
      {
        char c = std::cin.peek();
        if (std::isdigit(static_cast<unsigned char>(c)))
        {
          unsigned long long val;
          std::cin >> val;
          if (std::cin.fail())
          {
            std::cerr << "overflow\n";
            return 1;
          }
          nums.push_front(val);

          while (std::cin.peek() == ' ' || std::cin.peek() == '\t')
          {
            std::cin.get();
          }
        }
        else
        {
          break;
        }
      }

      if (std::cin.peek() == '\n')
      {
        std::cin.get();
      }

      List<unsigned long long> reversed;
      for (auto it = nums.cbegin(); it != nums.cend(); ++it)
      {
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
    for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
    {
      ordered.push_front(*it);
    }
    sequences = std::move(ordered);

    bool allEmpty = true;
    for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
    {
      if (!it->second.empty())
      {
        allEmpty = false;
        break;
      }
    }

    if (allEmpty)
    {
      bool first = true;
      for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
      {
        if (!first)
        {
          std::cout << ' ';
        }
        std::cout << it->first;
        first = false;
      }
      std::cout << "\n";
      std::cout << "0\n";
      return 0;
    }

    size_t maxLen = 0;
    for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
    {
      if (it->second.size() > maxLen)
      {
        maxLen = it->second.size();
      }
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
          {
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
      unsigned long long sum = 0;
      for (auto elem = col->cbegin(); elem != col->cend(); ++elem)
      {
        if (*elem > std::numeric_limits<unsigned long long>::max() - sum)
        {
          std::cerr << "overflow\n";
          return 1;
        }
        sum += *elem;
      }
      sums.push_back(sum);
    }

    bool first = true;
    for (auto it = sequences.cbegin(); it != sequences.cend(); ++it)
    {
      if (!first)
      {
        std::cout << ' ';
      }
      std::cout << it->first;
      first = false;
    }
    std::cout << "\n";

    for (auto col = columns.cbegin(); col != columns.cend(); ++col)
    {
      bool firstInRow = true;
      for (auto elem = col->cbegin(); elem != col->cend(); ++elem)
      {
        if (!firstInRow)
        {
          std::cout << ' ';
        }
        std::cout << *elem;
        firstInRow = false;
      }
      std::cout << "\n";
    }

    first = true;
    for (auto it = sums.cbegin(); it != sums.cend(); ++it)
    {
      if (!first)
      {
        std::cout << ' ';
      }
      std::cout << *it;
      first = false;
    }
    std::cout << "\n";

    return 0;
  }
  catch (const std::overflow_error& e)
  {
    std::cerr << "overflow\n";
    return 1;
  }
  catch (const std::exception& e)
  {
    std::cerr << e.what() << "\n";
    return 1;
  }
  catch (...)
  {
    std::cerr << "unknown error\n";
    return 1;
  }
}
