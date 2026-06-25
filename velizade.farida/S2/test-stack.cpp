#include <boost/test/unit_test.hpp>
#include "stack.hpp"

BOOST_AUTO_TEST_SUITE(StackTest)

BOOST_AUTO_TEST_CASE(stack_push_pop) {
  velizade::Stack<int> s;
  BOOST_CHECK(s.empty());
  BOOST_CHECK_EQUAL(s.size(), 0);

  s.push(10);
  s.push(20);
  BOOST_CHECK(!s.empty());
  BOOST_CHECK_EQUAL(s.size(), 2);
  BOOST_CHECK_EQUAL(s.top(), 20);

  int x = s.pop();
  BOOST_CHECK_EQUAL(x, 20);
  BOOST_CHECK_EQUAL(s.size(), 1);
  BOOST_CHECK_EQUAL(s.top(), 10);

  x = s.pop();
  BOOST_CHECK_EQUAL(x, 10);
  BOOST_CHECK(s.empty());
}

BOOST_AUTO_TEST_CASE(stack_exceptions) {
  velizade::Stack<int> s;
  BOOST_CHECK_THROW(s.pop(), std::runtime_error);
  BOOST_CHECK_THROW(s.top(), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()
