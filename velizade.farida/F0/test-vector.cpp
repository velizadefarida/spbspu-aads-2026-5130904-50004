#include <boost/test/unit_test.hpp>
#include "vector.hpp"

using namespace velizade;

BOOST_AUTO_TEST_SUITE(VectorTest)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  Vector<int> v;
  BOOST_CHECK_EQUAL(v.getSize(), 0);
  BOOST_CHECK(v.isEmpty());
  BOOST_CHECK_EQUAL(v.getCapacity(), 0);
}

BOOST_AUTO_TEST_CASE(ConstructorWithSize)
{
  const size_t N = 5;
  Vector<int> v(N);
  BOOST_CHECK_EQUAL(v.getSize(), N);
  BOOST_CHECK_EQUAL(v.getCapacity(), N);
  BOOST_CHECK(!v.isEmpty());
  for (size_t i = 0; i < N; ++i)
  {
    BOOST_CHECK_EQUAL(v[i], 0);
  }
}

BOOST_AUTO_TEST_CASE(ConstructorWithSizeAndValue)
{
  const size_t N = 3;
  Vector<int> v(N, 7);
  BOOST_CHECK_EQUAL(v.getSize(), N);
  for (size_t i = 0; i < N; ++i)
  {
    BOOST_CHECK_EQUAL(v[i], 7);
  }
}

BOOST_AUTO_TEST_CASE(PushBackAndSize)
{
  Vector<int> v;
  for (int i = 0; i < 10; ++i)
  {
    v.pushBack(i);
    BOOST_CHECK_EQUAL(v.getSize(), i + 1);
    BOOST_CHECK_EQUAL(v[i], i);
  }
  BOOST_CHECK(v.getCapacity() >= 10);
}

BOOST_AUTO_TEST_CASE(CopyConstructor)
{
  Vector<int> v1;
  v1.pushBack(1);
  v1.pushBack(2);
  Vector<int> v2 = v1;
  BOOST_CHECK_EQUAL(v2.getSize(), 2);
  BOOST_CHECK_EQUAL(v2[0], 1);
  BOOST_CHECK_EQUAL(v2[1], 2);
  v2[1] = 42;
  BOOST_CHECK_EQUAL(v1[1], 2);
  BOOST_CHECK_EQUAL(v2[1], 42);
}

BOOST_AUTO_TEST_CASE(MoveConstructor)
{
  Vector<int> v1;
  v1.pushBack(10);
  v1.pushBack(20);
  Vector<int> v2 = std::move(v1);
  BOOST_CHECK_EQUAL(v2.getSize(), 2);
  BOOST_CHECK_EQUAL(v2[0], 10);
  BOOST_CHECK_EQUAL(v2[1], 20);
  BOOST_CHECK_EQUAL(v1.getSize(), 0);
  BOOST_CHECK(v1.isEmpty());
}

BOOST_AUTO_TEST_CASE(CopyAssignment)
{
  Vector<int> v1;
  v1.pushBack(3);
  v1.pushBack(4);
  Vector<int> v2;
  v2 = v1;
  BOOST_CHECK_EQUAL(v2.getSize(), 2);
  BOOST_CHECK_EQUAL(v2[0], 3);
  BOOST_CHECK_EQUAL(v2[1], 4);
  v2[1] = 99;
  BOOST_CHECK_EQUAL(v1[1], 4);
}

BOOST_AUTO_TEST_CASE(MoveAssignment)
{
  Vector<int> v1;
  v1.pushBack(7);
  v1.pushBack(8);
  Vector<int> v2;
  v2 = std::move(v1);
  BOOST_CHECK_EQUAL(v2.getSize(), 2);
  BOOST_CHECK_EQUAL(v2[0], 7);
  BOOST_CHECK_EQUAL(v2[1], 8);
  BOOST_CHECK_EQUAL(v1.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(Swap)
{
  Vector<int> a;
  a.pushBack(1);
  a.pushBack(2);
  Vector<int> b;
  b.pushBack(3);
  a.swap(b);
  BOOST_CHECK_EQUAL(a.getSize(), 1);
  BOOST_CHECK_EQUAL(a[0], 3);
  BOOST_CHECK_EQUAL(b.getSize(), 2);
  BOOST_CHECK_EQUAL(b[0], 1);
  BOOST_CHECK_EQUAL(b[1], 2);
}

BOOST_AUTO_TEST_CASE(AtValid)
{
  Vector<int> v;
  v.pushBack(5);
  v.pushBack(6);
  BOOST_CHECK_EQUAL(v.at(0), 5);
  BOOST_CHECK_EQUAL(v.at(1), 6);
}

BOOST_AUTO_TEST_CASE(AtInvalid)
{
  Vector<int> v;
  v.pushBack(1);
  BOOST_CHECK_THROW(v.at(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(InsertSingle)
{
  Vector<int> v;
  v.pushBack(1);
  v.pushBack(3);
  v.insert(1, 2);
  BOOST_CHECK_EQUAL(v.getSize(), 3);
  BOOST_CHECK_EQUAL(v[0], 1);
  BOOST_CHECK_EQUAL(v[1], 2);
  BOOST_CHECK_EQUAL(v[2], 3);
}

BOOST_AUTO_TEST_CASE(InsertAtBeginning)
{
  Vector<int> v;
  v.pushBack(2);
  v.pushBack(3);
  v.insert(0, 1);
  BOOST_CHECK_EQUAL(v.getSize(), 3);
  BOOST_CHECK_EQUAL(v[0], 1);
  BOOST_CHECK_EQUAL(v[1], 2);
  BOOST_CHECK_EQUAL(v[2], 3);
}

BOOST_AUTO_TEST_CASE(InsertAtEnd)
{
  Vector<int> v;
  v.pushBack(1);
  v.pushBack(2);
  v.insert(v.getSize(), 3);
  BOOST_CHECK_EQUAL(v.getSize(), 3);
  BOOST_CHECK_EQUAL(v[0], 1);
  BOOST_CHECK_EQUAL(v[1], 2);
  BOOST_CHECK_EQUAL(v[2], 3);
}

BOOST_AUTO_TEST_CASE(InsertOutOfRange)
{
  Vector<int> v;
  v.pushBack(1);
  BOOST_CHECK_THROW(v.insert(2, 10), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(EraseSingle)
{
  Vector<int> v;
  v.pushBack(1);
  v.pushBack(2);
  v.pushBack(3);
  v.erase(1);
  BOOST_CHECK_EQUAL(v.getSize(), 2);
  BOOST_CHECK_EQUAL(v[0], 1);
  BOOST_CHECK_EQUAL(v[1], 3);
}

BOOST_AUTO_TEST_CASE(EraseOutOfRange)
{
  Vector<int> v;
  v.pushBack(1);
  BOOST_CHECK_THROW(v.erase(1), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(PopBack)
{
  Vector<int> v;
  v.pushBack(1);
  v.pushBack(2);
  v.popBack();
  BOOST_CHECK_EQUAL(v.getSize(), 1);
  BOOST_CHECK_EQUAL(v[0], 1);
  v.popBack();
  BOOST_CHECK_EQUAL(v.getSize(), 0);
}

BOOST_AUTO_TEST_CASE(PushFront)
{
  Vector<int> v;
  v.pushFront(1);
  v.pushFront(2);
  BOOST_CHECK_EQUAL(v.getSize(), 2);
  BOOST_CHECK_EQUAL(v[0], 2);
  BOOST_CHECK_EQUAL(v[1], 1);
}

BOOST_AUTO_TEST_CASE(IteratorBeginEnd)
{
  Vector<int> v;
  v.pushBack(1);
  v.pushBack(2);
  v.pushBack(3);
  int sum = 0;
  for (auto it = v.begin(); it != v.end(); ++it)
  {
    sum += *it;
  }
  BOOST_CHECK_EQUAL(sum, 6);
}

BOOST_AUTO_TEST_CASE(ConstIterator)
{
  Vector<int> v;
  v.pushBack(4);
  v.pushBack(5);
  const Vector<int>& cv = v;
  int sum = 0;
  for (auto it = cv.begin(); it != cv.end(); ++it)
  {
    sum += *it;
  }
  BOOST_CHECK_EQUAL(sum, 9);
}

BOOST_AUTO_TEST_SUITE_END()
