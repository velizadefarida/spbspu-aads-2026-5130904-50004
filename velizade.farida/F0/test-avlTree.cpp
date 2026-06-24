#include <boost/test/unit_test.hpp>
#include "avl_tree.hpp"
#include <vector>

BOOST_AUTO_TEST_SUITE(AVLTreeTest)

BOOST_AUTO_TEST_CASE(InsertAndFind)
{
  velizade::AVLTree<int, std::string> tree;
  BOOST_CHECK(tree.insert(1, "one"));
  BOOST_CHECK(tree.insert(2, "two"));
  BOOST_CHECK(tree.insert(3, "three"));
  BOOST_CHECK(!tree.insert(2, "duplicate"));

  std::string val;
  BOOST_CHECK(tree.find(2, val));
  BOOST_CHECK_EQUAL(val, "two");
  BOOST_CHECK(!tree.find(99));
}

BOOST_AUTO_TEST_CASE(Remove)
{
  velizade::AVLTree<int, std::string> tree;
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

BOOST_AUTO_TEST_CASE(GetAllSorted)
{
  velizade::AVLTree<int, std::string> tree;
  tree.insert(10, "ten");
  tree.insert(5, "five");
  tree.insert(15, "fifteen");
  tree.insert(3, "three");
  tree.insert(7, "seven");

  auto all = tree.getAll();
  BOOST_CHECK_EQUAL(all.getSize(), 5);
  std::vector<int> expected = {3, 5, 7, 10, 15};
  for (size_t i = 0; i < all.getSize(); ++i)
  {
    BOOST_CHECK_EQUAL(all[i].first, expected[i]);
  }
}

BOOST_AUTO_TEST_CASE(CopyAndMove)
{
  velizade::AVLTree<int, int> tree;
  for (int i = 0; i < 10; ++i)
  {
    tree.insert(i, i * 2);
  }

  velizade::AVLTree<int, int> copy = tree;
  BOOST_CHECK_EQUAL(copy.getAll().getSize(), 10);
  int val;
  BOOST_CHECK(copy.find(5, val));
  BOOST_CHECK_EQUAL(val, 10);

  velizade::AVLTree<int, int> moved = std::move(tree);
  BOOST_CHECK_EQUAL(moved.getAll().getSize(), 10);
  BOOST_CHECK(moved.find(3, val));
  BOOST_CHECK_EQUAL(val, 6);
  BOOST_CHECK_EQUAL(tree.getAll().getSize(), 0);
}

BOOST_AUTO_TEST_CASE(StringKeys)
{
  velizade::AVLTree<std::string, int> tree;
  tree.insert("apple", 10);
  tree.insert("banana", 20);
  tree.insert("cherry", 30);
  int val;
  BOOST_CHECK(tree.find("banana", val));
  BOOST_CHECK_EQUAL(val, 20);
  BOOST_CHECK(!tree.find("grape"));

  auto all = tree.getAll();
  BOOST_CHECK_EQUAL(all.getSize(), 3);
  BOOST_CHECK_EQUAL(all[0].first, "apple");
  BOOST_CHECK_EQUAL(all[1].first, "banana");
  BOOST_CHECK_EQUAL(all[2].first, "cherry");
}

BOOST_AUTO_TEST_CASE(ComplexOperations)
{
  velizade::AVLTree<int, int> tree;
  const int N = 100;
  for (int i = 0; i < N; ++i)
  {
    tree.insert(i, i);
  }
  BOOST_CHECK_EQUAL(tree.getAll().getSize(), N);

  for (int i = 0; i < N; ++i)
  {
    if (i % 2 == 0)
    {
      tree.remove(i);
    }
  }
  BOOST_CHECK_EQUAL(tree.getAll().getSize(), N / 2);

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
