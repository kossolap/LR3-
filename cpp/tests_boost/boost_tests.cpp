
#define BOOST_TEST_MODULE LR3BoostTests
#include "lr3/AVLTree.h"
#include "lr3/Array.h"
#include "lr3/DList.h"
#include "lr3/HashTable.h"
#include "lr3/List.h"
#include "lr3/Queue.h"
#include "lr3/Serialization.h"
#include "lr3/Stack.h"

#include <boost/test/unit_test.hpp>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <vector>

using namespace lr3;

BOOST_AUTO_TEST_CASE(ArrayTest) {
    Array array;
    BOOST_CHECK(array.empty());

    array.pushBack("A");
    array.pushBack("C");
    array.insert(1, "B");

    const std::vector<std::string> expected{"A", "B", "C"};
    BOOST_CHECK(array.toVector() == expected);
    BOOST_CHECK_EQUAL(array.at(1), "B");

    array.erase(1);
    BOOST_CHECK_EQUAL(array.size(), 2U);
    BOOST_CHECK_THROW(array.at(10), std::out_of_range);

    array.clear();
    BOOST_CHECK(array.empty());
}

BOOST_AUTO_TEST_CASE(ListTest) {
    List list;
    list.pushBack("A");
    list.pushBack("C");
    list.insertBefore("C", "B");
    list.insertAfter("C", "D");

    const std::vector<std::string> expected{"A", "B", "C", "D"};
    BOOST_CHECK(list.toVector() == expected);

    BOOST_CHECK(list.remove("B"));
    BOOST_CHECK(list.contains("C"));
    BOOST_CHECK_EQUAL(list.front(), "A");
    BOOST_CHECK_EQUAL(list.back(), "D");

    list.clear();
    BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(DoubleLinkedListTest) {
    DList list;
    list.pushBack("A");
    list.pushBack("C");
    list.insertBefore("C", "B");

    const std::vector<std::string> expected{"A", "B", "C"};
    const std::vector<std::string> reversed{"C", "B", "A"};

    BOOST_CHECK(list.toVector() == expected);
    BOOST_CHECK(list.toReverseVector() == reversed);

    BOOST_CHECK(list.remove("B"));
    BOOST_CHECK_EQUAL(list.size(), 2U);

    list.clear();
    BOOST_CHECK(list.empty());
}

BOOST_AUTO_TEST_CASE(QueueTest) {
    Queue queue;
    queue.push("A");
    queue.push("B");

    BOOST_CHECK_EQUAL(queue.front(), "A");
    BOOST_CHECK_EQUAL(queue.back(), "B");
    BOOST_CHECK_EQUAL(queue.pop(), "A");
    BOOST_CHECK_EQUAL(queue.pop(), "B");
    BOOST_CHECK(queue.empty());

    BOOST_CHECK_THROW(queue.pop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(StackTest) {
    Stack stack;
    stack.push("A");
    stack.push("B");

    BOOST_CHECK_EQUAL(stack.top(), "B");
    BOOST_CHECK_EQUAL(stack.pop(), "B");
    BOOST_CHECK_EQUAL(stack.pop(), "A");
    BOOST_CHECK(stack.empty());

    BOOST_CHECK_THROW(stack.pop(), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(HashTableTest) {
    HashTable table(5);

    table.insert(1, "one");
    table.insert(6, "six");

    BOOST_CHECK(table.contains(1));
    BOOST_CHECK(table.contains(6));
    BOOST_CHECK_EQUAL(table.get(1), "one");

    BOOST_CHECK(table.remove(1));
    BOOST_CHECK(!table.contains(1));
    BOOST_CHECK(table.contains(6));

    table.insert(6, "updated");
    BOOST_CHECK_EQUAL(table.get(6), "updated");

    table.clear();
    BOOST_CHECK(table.empty());
}

BOOST_AUTO_TEST_CASE(AVLTreeTest) {
    AVLTree tree;

    tree.insert("B");
    tree.insert("A");
    tree.insert("C");
    tree.insert("D");

    const std::vector<std::string> expected{"A", "B", "C", "D"};

    BOOST_CHECK(tree.toVector() == expected);
    BOOST_CHECK(tree.contains("B"));
    BOOST_CHECK(!tree.contains("X"));
    BOOST_CHECK_EQUAL(tree.size(), 4U);

    BOOST_CHECK(tree.remove("B"));
    BOOST_CHECK(!tree.contains("B"));

    tree.clear();
    BOOST_CHECK(tree.empty());
}

BOOST_AUTO_TEST_CASE(SerializationTest) {
    const std::string filename = "boost_serialization_test.bin";
    const std::vector<std::string> original{"Hello", "World", "Привет", ""};

    Serialization::saveBinary(filename, original);
    const auto restored = Serialization::loadBinary(filename);

    BOOST_CHECK(restored == original);
    std::remove(filename.c_str());
}
