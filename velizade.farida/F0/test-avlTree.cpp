#include <boost/test/unit_test.hpp>
#include "avl_tree.hpp"
#include <vector>

BOOST_AUTO_TEST_SUITE(AVLTreeTest)

BOOST_AUTO_TEST_CASE(InsertAndFind)
{
  AVLTree<int, std::string> tree;
  BOOST_CHECK(tree.insert(1, "one").second);
  BOOST_CHECK(tree.insert(2, "two").second);
  BOOST_CHECK(tree.insert(3, "three").second);
  BOOST_CHECK(!tree.insert(2, "duplicate").second);

  std::string val;
  BOOST_CHECK(tree.find(2, val));
  BOOST_CHECK_EQUAL(val, "two");
  BOOST_CHECK(!tree.find(99));
}

BOOST_AUTO_TEST_CASE(Remove)
{
  AVLTree<int, std::string> tree;
  tree.insert(5, "five");
  tree.insert(3, "three");
  tree.insert(7, "seven");
  tree.insert(1, "one");
  tree.insert(9, "nine");

  BOOST_CHECK(tree.remove(3));
  std::string val;
  BOOST_CHECK(!tree.find(3, val));
  BOOST_CHECK(tree.remove(1));
  BOOST_CHECK(!tree.find(1));
  BOOST_CHECK(!tree.remove(100));
}

BOOST_AUTO_TEST_CASE(IteratorOrder)
{
  AVLTree<int, std::string> tree;
  tree.insert(10, "ten");
  tree.insert(5, "five");
  tree.insert(15, "fifteen");
  tree.insert(3, "three");
  tree.insert(7, "seven");

  std::vector<int> expected = {3, 5, 7, 10, 15};
  size_t index = 0;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    BOOST_CHECK_EQUAL(it->first, expected[index]);
    ++index;
  }
  BOOST_CHECK_EQUAL(index, expected.size());
}

BOOST_AUTO_TEST_CASE(CopyAndMove)
{
  AVLTree<int, int> tree;
  for (int i = 0; i < 10; ++i)
  {
    tree.insert(i, i * 2);
  }

  AVLTree<int, int> copy = tree;
  BOOST_CHECK_EQUAL(std::distance(copy.begin(), copy.end()), 10);
  int val;
  BOOST_CHECK(copy.find(5, val));
  BOOST_CHECK_EQUAL(val, 10);

  AVLTree<int, int> moved = std::move(tree);
  BOOST_CHECK_EQUAL(std::distance(moved.begin(), moved.end()), 10);
  BOOST_CHECK(moved.find(3, val));
  BOOST_CHECK_EQUAL(val, 6);
  BOOST_CHECK_EQUAL(std::distance(tree.begin(), tree.end()), 0);
}

BOOST_AUTO_TEST_CASE(StringKeys)
{
  AVLTree<std::string, int> tree;
  tree.insert("apple", 10);
  tree.insert("banana", 20);
  tree.insert("cherry", 30);
  int val;
  BOOST_CHECK(tree.find("banana", val));
  BOOST_CHECK_EQUAL(val, 20);
  BOOST_CHECK(!tree.find("grape"));

  std::vector<std::string> expected = {"apple", "banana", "cherry"};
  size_t index = 0;
  for (auto it = tree.begin(); it != tree.end(); ++it)
  {
    BOOST_CHECK_EQUAL(it->first, expected[index]);
    ++index;
  }
}

BOOST_AUTO_TEST_CASE(ComplexOperations)
{
  AVLTree<int, int> tree;
  const int N = 100;
  for (int i = 0; i < N; ++i)
  {
    tree.insert(i, i);
  }
  BOOST_CHECK_EQUAL(std::distance(tree.begin(), tree.end()), N);

  for (int i = 0; i < N; ++i)
  {
    if (i % 2 == 0)
    {
      tree.remove(i);
    }
  }
  BOOST_CHECK_EQUAL(std::distance(tree.begin(), tree.end()), N / 2);

  for (int i = 0; i < N; ++i)
  {
    int val;
    bool found = tree.find(i, val);
    if (i % 2 == 0)
    {
      BOOST_CHECK(!found);
    }
    else
    {
      BOOST_CHECK(found);
      BOOST_CHECK_EQUAL(val, i);
    }
  }
}

BOOST_AUTO_TEST_SUITE_END()
