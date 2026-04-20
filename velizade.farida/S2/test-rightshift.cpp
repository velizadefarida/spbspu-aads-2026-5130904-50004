#include <boost/test/unit_test.hpp>
#include "math.hpp"

BOOST_AUTO_TEST_CASE(rightshift_correct) {
  BOOST_CHECK_EQUAL(velizade::calculateExpression("8 >> 2"), 2);
  BOOST_CHECK_EQUAL(velizade::calculateExpression("5 >> 1"), 2);
  BOOST_CHECK_EQUAL(velizade::calculateExpression("16 >> 3"), 2);
  BOOST_CHECK_EQUAL(velizade::calculateExpression("0 >> 5"), 0);
  BOOST_CHECK_EQUAL(velizade::calculateExpression("100 >> 0"), 100);
  BOOST_CHECK_EQUAL(velizade::calculateExpression("(4 + 2) >> 2"), 1);
  BOOST_CHECK_EQUAL(velizade::calculateExpression("15 >> 1 + 2"), 1);
}

BOOST_AUTO_TEST_CASE(rightshift_errors) {
  BOOST_CHECK_THROW(velizade::calculateExpression("100 >> -2"), std::runtime_error);
  BOOST_CHECK_THROW(velizade::calculateExpression("abc >> 2"), std::runtime_error);
}
