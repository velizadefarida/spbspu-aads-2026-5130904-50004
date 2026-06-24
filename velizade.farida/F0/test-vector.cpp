#include <boost/test/unit_test.hpp>
#include "vector.hpp"

BOOST_AUTO_TEST_SUITE(VectorTest)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  velizade::Vector<int> v;
  BOOST_CHECK_EQUAL(v.getSize(), 0);
  BOOST_CHECK_EQUAL(v.getCapacity(), 0);
}

BOOST_AUTO_TEST_CASE(ConstructorWithCapacity)
{
  velizade::Vector<int> v(5);
  BOOST_CHECK_EQUAL(v.getSize(), 0);
  BOOST_CHECK_EQUAL(v.getCapacity(), 5);
}

BOOST_AUTO_TEST_CASE(PushBackAndSize)
{
  velizade::Vector<int> v;
  for (int i = 0; i < 10; ++i)
  {
    v.push_back(i);
    BOOST_CHECK_EQUAL(v.getSize(), i + 1);
  }
  BOOST_CHECK(v.getCapacity() >= 10);
}

BOOST_AUTO_TEST_CASE(IndexOperator)
{
  velizade::Vector<int> v;
  v.push_back(42);
  v.push_back(100);
  BOOST_CHECK_EQUAL(v[0], 42);
  BOOST_CHECK_EQUAL(v[1], 100);
  v[1] = 200;
  BOOST_CHECK_EQUAL(v[1], 200);
}

BOOST_AUTO_TEST_CASE(Clear)
{
  velizade::Vector<int> v;
  v.push_back(1);
  v.push_back(2);
  v.clear();
  BOOST_CHECK_EQUAL(v.getSize(), 0);
  BOOST_CHECK(v.getCapacity() > 0);
}

BOOST_AUTO_TEST_CASE(PopBack)
{
  velizade::Vector<int> v;
  v.push_back(10);
  v.push_back(20);
  v.pop_back();
  BOOST_CHECK_EQUAL(v.getSize(), 1);
  BOOST_CHECK_EQUAL(v[0], 10);
  v.pop_back();
  BOOST_CHECK_EQUAL(v.getSize(), 0);
  v.pop_back();
  BOOST_CHECK_EQUAL(v.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(Back)
{
  velizade::Vector<int> v;
  v.push_back(5);
  v.push_back(7);
  BOOST_CHECK_EQUAL(v.back(), 7);
  v.back() = 9;
  BOOST_CHECK_EQUAL(v.back(), 9);
}

BOOST_AUTO_TEST_CASE(Iterators)
{
  velizade::Vector<int> v;
  for (int i = 0; i < 5; ++i)
  {
    v.push_back(i);
  }
  int sum = 0;
  for (auto it = v.begin(); it != v.end(); ++it)
  {
    sum += *it;
  }
  BOOST_CHECK_EQUAL(sum, 10);

  const velizade::Vector<int>& cv = v;
  int sumConst = 0;
  for (auto it = cv.begin(); it != cv.end(); ++it)
  {
    sumConst += *it;
  }
  BOOST_CHECK_EQUAL(sumConst, 10);
}

BOOST_AUTO_TEST_SUITE_END()
