#include <iostream>
#include <string>
#include <cctype>
#include <cstdlib>
#include "list.hpp"

using namespace velizade;

int main()
{
  List<std::pair<std::string, List<int>>> sequences;
  std::string name;

  while (std::cin >> name)
  {
    List<int> nums;

    while (std::cin.peek() == ' ' || std::cin.peek() == '\t')
      std::cin.get();

    while (std::cin.peek() != EOF && std::cin.peek() != '\n')
    {
      if (std::isdigit(std::cin.peek()))
      {
        int val;
        std::cin >> val;
        nums.push_front(val);
        while (std::cin.peek() == ' ' || std::cin.peek() == '\t')
          std::cin.get();
      }
      else
        break;
    }
    if (std::cin.peek() == '\n')
      std::cin.get();

    List<int> reversed;
    for (int v : nums)
      reversed.push_front(v);
    sequences.push_front({name, std::move(reversed)});
  }

  if (sequences.empty())
  {
    std::cout << "0" << std::endl;
    return 0;
  }

  List<std::pair<std::string, List<int>>> ordered;
  for (const auto& p : sequences)
    ordered.push_front(p);
  sequences = std::move(ordered);

  bool first = true;
  for (const auto& p : sequences)
  {
    if (!first) std::cout << ' ';
    std::cout << p.first;
    first = false;
  }
  std::cout << std::endl;

  List<LCIter<int>> iters;
  List<const List<int>*> lists;
  for (const auto& p : sequences)
  {
    lists.push_front(&p.second);
    iters.push_front(p.second.cbegin());
  }

  {
    List<LCIter<int>> iters_ok;
    for (auto it = iters.cbegin(); it != iters.cend(); ++it)
      iters_ok.push_front(*it);
    iters = std::move(iters_ok);

    List<const List<int>*> lists_ok;
    for (auto it = lists.cbegin(); it != lists.cend(); ++it)
      lists_ok.push_front(*it);
    lists = std::move(lists_ok);
  }

  List<int> sums;

  while (true)
  {
    List<int> column;
    auto it_it = iters.begin();
    auto list_it = lists.begin();
    bool any = false;

    while (it_it != iters.end() && list_it != lists.end())
    {
      const List<int>* lst = *list_it;
      LCIter<int>& iter = *it_it;
      if (iter != lst->cend())
      {
        column.push_front(*iter);
        ++iter;
        any = true;
      }
      ++it_it;
      ++list_it;
    }
    if (!any)
      break;

    first = true;
    for (int v : column)
    {
      if (!first) std::cout << ' ';
      std::cout << v;
      first = false;
    }
    std::cout << std::endl;
    int sum = 0;
    for (int v : column)
      sum += v;
    sums.push_front(sum);
  }

  if (sums.empty())
  {
    std::cerr << "Error: cannot compute sums" << std::endl;
    return 1;
  }

  first = true;
  for (int s : sums)
  {
    if (!first) std::cout << ' ';
    std::cout << s;
    first = false;
  }
  std::cout << std::endl;

  return 0;
}
