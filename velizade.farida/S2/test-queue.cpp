#include <boost/test/unit_test.hpp>
#include "queue.hpp"

BOOST_AUTO_TEST_SUITE(QueueTests)

BOOST_AUTO_TEST_CASE(queue_push_pop) {
  velizade::Queue<int> q;
  BOOST_CHECK(q.empty());
  BOOST_CHECK_EQUAL(q.size(), 0);

  q.push(1);
  q.push(2);
  q.push(3);
  BOOST_CHECK(!q.empty());
  BOOST_CHECK_EQUAL(q.size(), 3);
  BOOST_CHECK_EQUAL(q.front(), 1);

  int x = q.pop();
  BOOST_CHECK_EQUAL(x, 1);
  BOOST_CHECK_EQUAL(q.size(), 2);
  BOOST_CHECK_EQUAL(q.front(), 2);

  x = q.pop();
  BOOST_CHECK_EQUAL(x, 2);
  x = q.pop();
  BOOST_CHECK_EQUAL(x, 3);
  BOOST_CHECK(q.empty());
}

BOOST_AUTO_TEST_CASE(queue_exceptions) {
  velizade::Queue<int> q;
  BOOST_CHECK_THROW(q.pop(), std::runtime_error);
  BOOST_CHECK_THROW(q.front(), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()
