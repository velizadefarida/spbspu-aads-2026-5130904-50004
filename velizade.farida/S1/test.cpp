#define BOOST_TEST_MODULE ListTests
#include <boost/test/included/unit_test.hpp>
#include "list.hpp"

using namespace velizade;

BOOST_AUTO_TEST_CASE(test_push_front_front)
{
  List<int> lst;
  lst.push_front(1);
  lst.push_front(2);
  BOOST_CHECK_EQUAL(lst.front(), 2);
  BOOST_CHECK_EQUAL(lst.size(), 2);
}

BOOST_AUTO_TEST_CASE(test_pop_front)
{
  List<int> lst;
  lst.push_front(1);
  lst.push_front(2);
  lst.pop_front();
  BOOST_CHECK_EQUAL(lst.front(), 1);
  lst.pop_front();
  BOOST_CHECK(lst.empty());
}

BOOST_AUTO_TEST_CASE(test_back)
{
  List<int> lst;
  lst.push_front(3);
  lst.push_front(2);
  lst.push_front(1);
  BOOST_CHECK_EQUAL(lst.back(), 3);
}

BOOST_AUTO_TEST_CASE(test_insert_after)
{
  List<int> lst;
  lst.push_front(3);
  lst.push_front(1);
  auto it = lst.begin();
  ++it;
  lst.insert_after(it, 2);
  auto it2 = lst.begin();
  BOOST_CHECK_EQUAL(*it2, 1);
  ++it2;
  BOOST_CHECK_EQUAL(*it2, 2);
  ++it2;
  BOOST_CHECK_EQUAL(*it2, 3);
}

BOOST_AUTO_TEST_CASE(test_erase_after)
{
  List<int> lst;
  lst.push_front(3);
  lst.push_front(2);
  lst.push_front(1);
  auto it = lst.begin();
  lst.erase_after(it);
  BOOST_CHECK_EQUAL(lst.size(), 2);
  auto it2 = lst.begin();
  BOOST_CHECK_EQUAL(*it2, 1);
  ++it2;
  BOOST_CHECK_EQUAL(*it2, 3);
}

BOOST_AUTO_TEST_CASE(test_copy)
{
  List<int> lst;
  lst.push_front(1);
  lst.push_front(2);
  List<int> lst2 = lst;
  BOOST_CHECK_EQUAL(lst2.size(), 2);
  BOOST_CHECK_EQUAL(lst2.front(), 2);
  lst2.pop_front();
  BOOST_CHECK_EQUAL(lst.front(), 2);
}

BOOST_AUTO_TEST_CASE(test_move)
{
  List<int> lst;
  lst.push_front(1);
  lst.push_front(2);
  List<int> lst2 = std::move(lst);
  BOOST_CHECK_EQUAL(lst2.size(), 2);
  BOOST_CHECK(lst.empty());
}

BOOST_AUTO_TEST_CASE(test_const_iterators)
{
  List<int> lst;
  lst.push_front(1);
  lst.push_front(2);
  const List<int>& clst = lst;
  auto it = clst.cbegin();
  BOOST_CHECK_EQUAL(*it, 2);
  ++it;
  BOOST_CHECK_EQUAL(*it, 1);
}
