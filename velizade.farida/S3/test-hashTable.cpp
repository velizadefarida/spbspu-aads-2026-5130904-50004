#include <boost/test/unit_test.hpp>
#include "hashTable.hpp"
#include <string>

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(testAddWithinCap)
{
  velizade::HashTable<int, int> ht(2, 3, 2);
  ht.add(1, 100);
  ht.add(2, 200);
  ht.add(3, 300);

  BOOST_CHECK(ht.has(1));
  BOOST_CHECK(ht.has(2));
  BOOST_CHECK(ht.has(3));
  BOOST_CHECK_EQUAL(ht.getSize(), 3);
}

BOOST_AUTO_TEST_CASE(testOverflowSpare)
{
  velizade::HashTable<int, int> ht(1, 2, 2);
  ht.add(1, 10);
  ht.add(2, 20);
  ht.add(3, 30);
  ht.add(4, 40);

  BOOST_CHECK(ht.has(1));
  BOOST_CHECK(ht.has(2));
  BOOST_CHECK(ht.has(3));
  BOOST_CHECK(ht.has(4));
  BOOST_CHECK_EQUAL(ht.getSize(), 4);
}

BOOST_AUTO_TEST_CASE(testFullException)
{
  velizade::HashTable<int, int> ht(1, 2, 2);
  ht.add(1, 10);
  ht.add(2, 20);
  ht.add(3, 30);
  ht.add(4, 40);

  BOOST_CHECK_THROW(ht.add(5, 50), std::overflow_error);
}

BOOST_AUTO_TEST_CASE(testFindOverflow)
{
  velizade::HashTable<int, int> ht(1, 2, 2);
  ht.add(1, 10);
  ht.add(2, 20);
  ht.add(3, 30);

  auto it = ht.find(2);
  BOOST_REQUIRE(it != ht.end());
  BOOST_CHECK_EQUAL(it->second, 20);

  auto it2 = ht.find(3);
  BOOST_REQUIRE(it2 != ht.end());
  BOOST_CHECK_EQUAL(it2->second, 30);

  auto it3 = ht.find(99);
  BOOST_CHECK(it3 == ht.end());
}

BOOST_AUTO_TEST_CASE(testDrop)
{
  velizade::HashTable<std::string, int> ht(2, 2, 2);
  ht.add("a", 1);
  ht.add("b", 2);
  ht.add("c", 3);

  BOOST_CHECK(ht.drop("b"));
  BOOST_CHECK(!ht.has("b"));
  BOOST_CHECK(ht.has("a"));
  BOOST_CHECK(ht.has("c"));
  BOOST_CHECK_EQUAL(ht.getSize(), 2);

  BOOST_CHECK(!ht.drop("x"));
  BOOST_CHECK_EQUAL(ht.getSize(), 2);
}

BOOST_AUTO_TEST_CASE(testRehash)
{
  velizade::HashTable<int, int> ht(1, 2, 2);
  ht.add(1, 10);
  ht.add(2, 20);
  ht.add(3, 30);
  ht.add(4, 40);

  ht.rehash(4);
  BOOST_CHECK_EQUAL(ht.getBucketCount(), 4);
  BOOST_CHECK_EQUAL(ht.getSize(), 4);
  BOOST_CHECK(ht.has(1));
  BOOST_CHECK(ht.has(2));
  BOOST_CHECK(ht.has(3));
  BOOST_CHECK(ht.has(4));

  BOOST_CHECK_THROW(ht.rehash(0), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(testCopySwap)
{
  velizade::HashTable<std::string, int> ht1(2, 2, 2);
  ht1.add("a", 1);
  ht1.add("b", 2);

  velizade::HashTable<std::string, int> ht2 = ht1;
  BOOST_CHECK(ht2.has("a"));
  BOOST_CHECK(ht2.has("b"));
  BOOST_CHECK_EQUAL(ht2.getSize(), 2);

  ht2.drop("b");
  BOOST_CHECK(!ht2.has("b"));
  BOOST_CHECK(ht1.has("b"));
}

BOOST_AUTO_TEST_CASE(testIterator)
{
  velizade::HashTable<std::string, int> ht(2, 2, 2);
  ht.add("A", 10);
  ht.add("B", 20);
  ht.add("C", 30);

  size_t count = 0;
  for (auto it = ht.cbegin(); it != ht.cend(); ++it)
  {
    ++count;
    BOOST_CHECK(it->first == "A" || it->first == "B" || it->first == "C");
  }
  BOOST_CHECK_EQUAL(count, 3);

  for (auto it = ht.begin(); it != ht.end(); ++it)
  {
    it->second += 5;
  }

  for (auto it = ht.cbegin(); it != ht.cend(); ++it)
  {
    if (it->first == "A")
    {
      BOOST_CHECK_EQUAL(it->second, 15);
    }
    else if (it->first == "B")
    {
      BOOST_CHECK_EQUAL(it->second, 25);
    }
    else if (it->first == "C")
    {
      BOOST_CHECK_EQUAL(it->second, 35);
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()
