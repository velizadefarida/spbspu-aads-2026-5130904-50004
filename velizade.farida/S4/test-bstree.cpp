#include <boost/test/unit_test.hpp>
#include "bstree.hpp"
#include <vector>
#include <algorithm>

BOOST_AUTO_TEST_SUITE(BSTreeTests)

BOOST_AUTO_TEST_CASE(DefaultConstructor)
{
  velizade::BSTree<int, std::string> tree;
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(PushAndGet)
{
  velizade::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  BOOST_CHECK_EQUAL(tree.get(5), "five");
  BOOST_CHECK_EQUAL(tree.get(3), "three");
  BOOST_CHECK_EQUAL(tree.get(7), "seven");
}

BOOST_AUTO_TEST_CASE(GetNonexistent)
{
  velizade::BSTree<int, std::string> tree;
  tree.push(5, "five");
  BOOST_CHECK_THROW(tree.get(10), std::out_of_range);
  BOOST_CHECK_THROW(tree.get(0), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(Erase)
{
  velizade::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(1, "one");

  std::string val = tree.drop(1);
  BOOST_CHECK_EQUAL(val, "one");
  BOOST_CHECK_THROW(tree.get(1), std::out_of_range);

  val = tree.drop(5);
  BOOST_CHECK_EQUAL(val, "five");
  BOOST_CHECK_THROW(tree.get(5), std::out_of_range);

  val = tree.drop(7);
  BOOST_CHECK_EQUAL(val, "seven");
  BOOST_CHECK_THROW(tree.get(7), std::out_of_range);

  val = tree.drop(3);
  BOOST_CHECK_EQUAL(val, "three");
  BOOST_CHECK_THROW(tree.get(3), std::out_of_range);
  BOOST_CHECK(tree.empty());
}

BOOST_AUTO_TEST_CASE(EraseNonexistent)
{
  velizade::BSTree<int, std::string> tree;
  tree.push(5, "five");
  BOOST_CHECK_THROW(tree.drop(10), std::out_of_range);
  BOOST_CHECK_THROW(tree.drop(0), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(Clear)
{
  velizade::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");

  tree.drop(5);
  tree.drop(3);
  tree.drop(7);
  BOOST_CHECK(tree.empty());
  BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(HeightEmpty)
{
  velizade::BSTree<int, std::string> tree;
  BOOST_CHECK_EQUAL(tree.height(), 0);
}

BOOST_AUTO_TEST_CASE(HeightSimple)
{
  velizade::BSTree<int, std::string> tree;
  tree.push(5, "five");
  BOOST_CHECK_EQUAL(tree.height(), 1);

  tree.push(3, "three");
  BOOST_CHECK_EQUAL(tree.height(), 2);

  tree.push(7, "seven");
  BOOST_CHECK_EQUAL(tree.height(), 2);

  tree.push(1, "one");
  BOOST_CHECK_EQUAL(tree.height(), 3);

  tree.push(9, "nine");
  BOOST_CHECK_EQUAL(tree.height(), 3);
}

BOOST_AUTO_TEST_CASE(HeightOfSubtree)
{
  velizade::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");
  tree.push(1, "one");
  tree.push(9, "nine");

  auto it = tree.begin();
  BOOST_CHECK_EQUAL(tree.height(it), 1);
  ++it;
  BOOST_CHECK_EQUAL(tree.height(it), 2);
  ++it;
  BOOST_CHECK_EQUAL(tree.height(it), 3);
  ++it;
  BOOST_CHECK_EQUAL(tree.height(it), 2);
  ++it;
  BOOST_CHECK_EQUAL(tree.height(it), 1);
}

BOOST_AUTO_TEST_CASE(RotateLeftBasic)
{
  velizade::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");

  auto it7 = tree.begin(); ++it7; ++it7;
  auto res = tree.rotateLeft(it7);
  BOOST_CHECK_EQUAL(res.key(), 7);
  BOOST_CHECK_EQUAL(tree.height(), 2);

  std::vector<int> keys;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    keys.push_back(it.key());
  }
  std::vector<int> expected = {3, 5, 7};
  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(RotateRightBasic)
{
  velizade::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");

  auto it3 = tree.begin();
  auto res = tree.rotateRight(it3);
  BOOST_CHECK_EQUAL(res.key(), 3);
  BOOST_CHECK_EQUAL(tree.height(), 2);

  std::vector<int> keys;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    keys.push_back(it.key());
  }
  std::vector<int> expected = {3, 5, 7};
  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(RotateLargeLeft)
{
  velizade::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(4, "four");

  auto it4 = tree.begin(); ++it4; ++it4;
  auto res = tree.rotateLargeLeft(it4);
  BOOST_CHECK_EQUAL(res.key(), 4);
  BOOST_CHECK_EQUAL(tree.height(), 2);

  std::vector<int> keys;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    keys.push_back(it.key());
  }
  std::vector<int> expected = {3, 4, 5};
  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(RotateLargeRight)
{
  velizade::BSTree<int, std::string> tree;
  tree.push(3, "three");
  tree.push(5, "five");
  tree.push(4, "four");

  auto it4 = tree.begin(); ++it4; ++it4;
  auto res = tree.rotateLargeRight(it4);
  BOOST_CHECK_EQUAL(res.key(), 4);
  BOOST_CHECK_EQUAL(tree.height(), 2);

  std::vector<int> keys;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    keys.push_back(it.key());
  }
  std::vector<int> expected = {3, 4, 5};
  BOOST_CHECK_EQUAL_COLLECTIONS(keys.begin(), keys.end(), expected.begin(), expected.end());
}

BOOST_AUTO_TEST_CASE(RotateInvalidIterator)
{
  velizade::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");

  auto it_end = tree.end();
  auto res1 = tree.rotateLeft(it_end);
  BOOST_CHECK(res1.getNode() == it_end.getNode());

  auto res2 = tree.rotateRight(it_end);
  BOOST_CHECK(res2.getNode() == it_end.getNode());

  auto res3 = tree.rotateLargeLeft(it_end);
  BOOST_CHECK(res3.getNode() == it_end.getNode());

  auto res4 = tree.rotateLargeRight(it_end);
  BOOST_CHECK(res4.getNode() == it_end.getNode());
}

BOOST_AUTO_TEST_CASE(Find)
{
  velizade::BSTree<int, std::string> tree;
  tree.push(5, "five");
  tree.push(3, "three");
  tree.push(7, "seven");

  BOOST_CHECK(tree.contains(5));
  BOOST_CHECK(tree.contains(3));
  BOOST_CHECK(tree.contains(7));
  BOOST_CHECK(!tree.contains(1));
  BOOST_CHECK(!tree.contains(9));
}

BOOST_AUTO_TEST_SUITE_END()
