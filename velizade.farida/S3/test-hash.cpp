#define BOOST_TEST_MODULE S3
#include <boost/test/unit_test.hpp>
#include "hash_table.hpp"
#include <string>
#include <vector>
#include <algorithm>

BOOST_AUTO_TEST_SUITE(HashTableTests)

BOOST_AUTO_TEST_CASE(test_add_and_has)
{
    velizade::HashTable<int, std::string> ht;
    ht.add(1, "one");
    ht.add(2, "two");
    BOOST_CHECK(ht.has(1));
    BOOST_CHECK(ht.has(2));
    BOOST_CHECK(!ht.has(3));

    auto* cell = ht.find(1);
    BOOST_REQUIRE(cell != nullptr);
    BOOST_CHECK_EQUAL(cell->value, "one");
}

BOOST_AUTO_TEST_CASE(test_add_duplicate)
{
    velizade::HashTable<int, std::string> ht;
    ht.add(1, "one");
    BOOST_CHECK_THROW(ht.add(1, "another"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(test_drop)
{
    velizade::HashTable<int, std::string> ht;
    ht.add(1, "one");
    ht.add(2, "two");
    std::string val = ht.drop(1);
    BOOST_CHECK_EQUAL(val, "one");
    BOOST_CHECK(!ht.has(1));
    BOOST_CHECK(ht.has(2));
}

BOOST_AUTO_TEST_CASE(test_drop_missing)
{
    velizade::HashTable<int, std::string> ht;
    ht.add(1, "one");
    BOOST_CHECK_THROW(ht.drop(2), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(test_rehash)
{
    velizade::HashTable<int, std::string> ht(2, 2);
    ht.add(1, "one");
    ht.add(2, "two");
    ht.add(3, "three");
    ht.rehash(4);
    BOOST_CHECK(ht.has(1));
    BOOST_CHECK(ht.has(2));
    BOOST_CHECK(ht.has(3));
    auto* cell = ht.find(3);
    BOOST_REQUIRE(cell != nullptr);
    BOOST_CHECK_EQUAL(cell->value, "three");
}

BOOST_AUTO_TEST_CASE(test_overflow)
{
    velizade::HashTable<int, int> ht(1, 2);
    ht.add(1, 10);
    ht.add(2, 20);
    ht.add(3, 30);
    BOOST_CHECK(ht.has(1));
    BOOST_CHECK(ht.has(2));
    BOOST_CHECK(ht.has(3));
    BOOST_CHECK_EQUAL(ht.find(3)->value, 30);
}

BOOST_AUTO_TEST_CASE(test_iterators)
{
    velizade::HashTable<int, std::string> ht;
    ht.add(1, "one");
    ht.add(2, "two");
    ht.add(3, "three");
    std::vector<int> keys;
    for (auto it = ht.begin(); it != ht.end(); ++it)
    {
        keys.push_back((*it).first);
    }
    BOOST_CHECK_EQUAL(keys.size(), 3);
    std::sort(keys.begin(), keys.end());
    BOOST_CHECK_EQUAL(keys[0], 1);
    BOOST_CHECK_EQUAL(keys[1], 2);
    BOOST_CHECK_EQUAL(keys[2], 3);
}

BOOST_AUTO_TEST_SUITE_END()
