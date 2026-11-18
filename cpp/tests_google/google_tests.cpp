
#include "lr3/AVLTree.h"
#include "lr3/Array.h"
#include "lr3/DList.h"
#include "lr3/HashTable.h"
#include "lr3/List.h"
#include "lr3/Queue.h"
#include "lr3/Serialization.h"
#include "lr3/Stack.h"

#include <cstdio>
#include <gtest/gtest.h>
#include <stdexcept>
#include <string>
#include <vector>

using namespace lr3;

TEST(ArrayGoogleTest, Operations) {
    Array array;
    EXPECT_TRUE(array.empty());

    array.pushBack("A");
    array.pushBack("C");
    array.insert(1, "B");

    const std::vector<std::string> expected{"A", "B", "C"};
    EXPECT_EQ(array.toVector(), expected);
    EXPECT_EQ(array.at(1), "B");

    array.erase(1);
    EXPECT_EQ(array.size(), 2U);
    EXPECT_THROW(array.at(10), std::out_of_range);

    array.clear();
    EXPECT_TRUE(array.empty());
}

TEST(ListGoogleTest, Operations) {
    List list;
    list.pushBack("A");
    list.pushBack("C");
    list.insertBefore("C", "B");
    list.insertAfter("C", "D");

    const std::vector<std::string> expected{"A", "B", "C", "D"};
    EXPECT_EQ(list.toVector(), expected);

    EXPECT_TRUE(list.remove("B"));
    EXPECT_TRUE(list.contains("C"));
    EXPECT_EQ(list.front(), "A");
    EXPECT_EQ(list.back(), "D");

    list.clear();
    EXPECT_TRUE(list.empty());
}

TEST(DoubleLinkedListGoogleTest, Operations) {
    DList list;
    list.pushBack("A");
    list.pushBack("C");
    list.insertBefore("C", "B");

    const std::vector<std::string> expected{"A", "B", "C"};
    const std::vector<std::string> reversed{"C", "B", "A"};

    EXPECT_EQ(list.toVector(), expected);
    EXPECT_EQ(list.toReverseVector(), reversed);

    EXPECT_TRUE(list.remove("B"));
    EXPECT_EQ(list.size(), 2U);

    list.clear();
    EXPECT_TRUE(list.empty());
}

TEST(QueueGoogleTest, Operations) {
    Queue queue;
    queue.push("A");
    queue.push("B");

    EXPECT_EQ(queue.front(), "A");
    EXPECT_EQ(queue.back(), "B");
    EXPECT_EQ(queue.pop(), "A");
    EXPECT_EQ(queue.pop(), "B");
    EXPECT_TRUE(queue.empty());

    EXPECT_THROW(queue.pop(), std::out_of_range);
}

TEST(StackGoogleTest, Operations) {
    Stack stack;
    stack.push("A");
    stack.push("B");

    EXPECT_EQ(stack.top(), "B");
    EXPECT_EQ(stack.pop(), "B");
    EXPECT_EQ(stack.pop(), "A");
    EXPECT_TRUE(stack.empty());

    EXPECT_THROW(stack.pop(), std::out_of_range);
}

TEST(HashTableGoogleTest, Operations) {
    HashTable table(5);

    table.insert(1, "one");
    table.insert(6, "six");

    EXPECT_TRUE(table.contains(1));
    EXPECT_TRUE(table.contains(6));
    EXPECT_EQ(table.get(1), "one");

    EXPECT_TRUE(table.remove(1));
    EXPECT_FALSE(table.contains(1));
    EXPECT_TRUE(table.contains(6));

    table.insert(6, "updated");
    EXPECT_EQ(table.get(6), "updated");

    table.clear();
    EXPECT_TRUE(table.empty());
}

TEST(AVLTreeGoogleTest, Operations) {
    AVLTree tree;

    tree.insert("B");
    tree.insert("A");
    tree.insert("C");
    tree.insert("D");

    const std::vector<std::string> expected{"A", "B", "C", "D"};

    EXPECT_EQ(tree.toVector(), expected);
    EXPECT_TRUE(tree.contains("B"));
    EXPECT_FALSE(tree.contains("X"));
    EXPECT_EQ(tree.size(), 4U);

    EXPECT_TRUE(tree.remove("B"));
    EXPECT_FALSE(tree.contains("B"));

    tree.clear();
    EXPECT_TRUE(tree.empty());
}

TEST(SerializationGoogleTest, BinaryRoundTrip) {
    const std::string filename = "gtest_serialization_test.bin";
    const std::vector<std::string> original{"Hello", "World", "Привет", ""};

    Serialization::saveBinary(filename, original);
    const auto restored = Serialization::loadBinary(filename);

    EXPECT_EQ(restored, original);
    std::remove(filename.c_str());
}
