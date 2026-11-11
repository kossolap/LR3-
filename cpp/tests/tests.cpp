#include "lr3/AVLTree.h"
#include "lr3/Array.h"
#include "lr3/DList.h"
#include "lr3/HashTable.h"
#include "lr3/List.h"
#include "lr3/Queue.h"
#include "lr3/Serialization.h"
#include "lr3/Stack.h"

#include <catch2/catch_test_macros.hpp>
#include <cstdio>
#include <functional>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using namespace lr3;

// ==================== ARRAY ====================

TEST_CASE("Array constructors and capacity", "[array]") {
    Array array;
    REQUIRE(array.empty());
    REQUIRE(array.size() == 0);
    REQUIRE(array.capacity() == 0);

    Array reserved(10);
    REQUIRE(reserved.empty());
    REQUIRE(reserved.capacity() == 10);

    reserved.pushBack("One");
    REQUIRE(reserved.size() == 1);
    REQUIRE(reserved.capacity() == 10);

    Array zeroCapacity(0);
    REQUIRE(zeroCapacity.capacity() == 0);

    zeroCapacity.pushBack("First");
    REQUIRE(zeroCapacity.at(0) == "First");
    REQUIRE(zeroCapacity.capacity() >= 1);
}

TEST_CASE("Array insertion and deletion", "[array]") {
    Array array;

    array.pushBack("A");
    array.pushBack("C");
    array.insert(1, "B");

    REQUIRE(array.toVector() == std::vector<std::string>{"A", "B", "C"});

    array.insert(0, "Start");
    array.insert(array.size(), "End");

    REQUIRE(array.at(0) == "Start");
    REQUIRE(array.at(array.size() - 1) == "End");

    array.erase(0);
    array.erase(array.size() - 1);

    REQUIRE(array.toVector() == std::vector<std::string>{"A", "B", "C"});

    array.erase(1);
    REQUIRE(array.toVector() == std::vector<std::string>{"A", "C"});
}

TEST_CASE("Array exceptions", "[array]") {
    Array array;

    REQUIRE_THROWS_AS(array.at(0), std::out_of_range);
    REQUIRE_THROWS_AS(array.set(0, "X"), std::out_of_range);
    REQUIRE_THROWS_AS(array.erase(0), std::out_of_range);
    REQUIRE_THROWS_AS(array.insert(1, "X"), std::out_of_range);

    array.pushBack("A");

    REQUIRE_THROWS_AS(array.at(1), std::out_of_range);
    REQUIRE_THROWS_AS(array.set(1, "B"), std::out_of_range);
    REQUIRE_THROWS_AS(array.erase(1), std::out_of_range);
    REQUIRE_THROWS_AS(array.insert(3, "X"), std::out_of_range);
}

TEST_CASE("Array copying and moving", "[array]") {
    Array original;

    original.pushBack("One");
    original.pushBack("Two");

    Array copied(original);

    REQUIRE(copied.toVector() == original.toVector());

    copied.set(0, "Changed");
    REQUIRE(original.at(0) == "One");

    Array assigned;
    assigned = original;

    REQUIRE(assigned.toVector() == original.toVector());

    assigned = assigned;
    REQUIRE(assigned.toVector() == original.toVector());

    Array moved(std::move(copied));

    REQUIRE(moved.size() == 2);
    REQUIRE(moved.at(0) == "Changed");
    REQUIRE(copied.empty());

    Array moveAssigned;
    moveAssigned = std::move(moved);

    REQUIRE(moveAssigned.size() == 2);
    REQUIRE(moved.empty());

    moveAssigned = std::move(moveAssigned);
    REQUIRE(moveAssigned.size() == 2);

    copied.pushBack("After move");
    REQUIRE(copied.at(0) == "After move");
}

TEST_CASE("Array clear and reuse", "[array]") {
    Array array(5);

    array.pushBack("A");
    array.pushBack("B");

    array.clear();

    REQUIRE(array.empty());
    REQUIRE(array.capacity() == 5);

    array.pushBack("C");
    REQUIRE(array.at(0) == "C");
}

// ==================== LIST ====================

TEST_CASE("List insertion positions", "[list]") {
    List list;

    list.pushFront("B");
    list.pushFront("A");
    list.pushBack("D");

    list.insertBefore("B", "Before B");
    list.insertAfter("B", "After B");
    list.insertBefore("D", "Before D");
    list.insertAfter("D", "After D");

    REQUIRE(list.toVector() ==
            std::vector<std::string>{"A", "Before B", "B", "After B", "Before D", "D", "After D"});
}

TEST_CASE("List removal positions", "[list]") {
    List list;

    REQUIRE_FALSE(list.remove("Missing"));

    list.pushBack("A");
    REQUIRE(list.remove("A"));
    REQUIRE(list.empty());

    list.pushBack("A");
    list.pushBack("B");
    list.pushBack("C");

    REQUIRE(list.remove("B"));
    REQUIRE(list.toVector() == std::vector<std::string>{"A", "C"});

    REQUIRE(list.remove("A"));
    REQUIRE(list.front() == "C");

    list.pushBack("D");
    REQUIRE(list.remove("D"));
    REQUIRE(list.back() == "C");

    REQUIRE_FALSE(list.remove("Missing"));
}

TEST_CASE("List exceptions and empty operations", "[list]") {
    List list;

    REQUIRE_THROWS_AS(list.popFront(), std::out_of_range);
    REQUIRE_THROWS_AS(list.popBack(), std::out_of_range);
    REQUIRE_THROWS_AS(list.front(), std::out_of_range);
    REQUIRE_THROWS_AS(list.back(), std::out_of_range);

    REQUIRE_THROWS_AS(list.insertBefore("X", "Y"), std::out_of_range);

    REQUIRE_THROWS_AS(list.insertAfter("X", "Y"), std::out_of_range);

    list.pushBack("A");

    REQUIRE_THROWS_AS(list.insertBefore("Missing", "B"), std::out_of_range);

    REQUIRE_THROWS_AS(list.insertAfter("Missing", "B"), std::out_of_range);
}

TEST_CASE("List copying and assignment", "[list]") {
    List list;
    list.pushBack("A");
    list.pushBack("B");

    List copied(list);
    REQUIRE(copied.toVector() == list.toVector());

    List assigned;
    assigned.pushBack("Old");
    assigned = list;

    REQUIRE(assigned.toVector() == list.toVector());

    assigned = assigned;
    REQUIRE(assigned.toVector() == list.toVector());

    copied.clear();
    REQUIRE(list.size() == 2);

    List emptyList;
    List emptyCopy(emptyList);
    REQUIRE(emptyCopy.empty());

    assigned = emptyList;
    REQUIRE(assigned.empty());
}

// ==================== DOUBLE LINKED LIST ====================

TEST_CASE("DList insertion positions", "[dlist]") {
    DList list;

    list.pushFront("B");
    list.pushFront("A");
    list.pushBack("D");

    list.insertBefore("B", "Before B");
    list.insertAfter("B", "After B");
    list.insertBefore("D", "Before D");
    list.insertAfter("D", "After D");

    REQUIRE(list.toVector() ==
            std::vector<std::string>{"A", "Before B", "B", "After B", "Before D", "D", "After D"});

    REQUIRE(list.toReverseVector() ==
            std::vector<std::string>{"After D", "D", "Before D", "After B", "B", "Before B", "A"});
}

TEST_CASE("DList removal positions", "[dlist]") {
    DList list;

    REQUIRE_FALSE(list.remove("Missing"));

    list.pushBack("A");
    REQUIRE(list.remove("A"));
    REQUIRE(list.empty());

    list.pushBack("A");
    list.pushBack("B");
    list.pushBack("C");

    REQUIRE(list.remove("B"));
    REQUIRE(list.toVector() == std::vector<std::string>{"A", "C"});

    REQUIRE(list.remove("A"));
    REQUIRE(list.front() == "C");

    list.pushBack("D");
    REQUIRE(list.remove("D"));
    REQUIRE(list.back() == "C");
}

TEST_CASE("DList exceptions and empty operations", "[dlist]") {
    DList list;

    REQUIRE_THROWS_AS(list.popFront(), std::out_of_range);
    REQUIRE_THROWS_AS(list.popBack(), std::out_of_range);
    REQUIRE_THROWS_AS(list.front(), std::out_of_range);
    REQUIRE_THROWS_AS(list.back(), std::out_of_range);

    REQUIRE_THROWS_AS(list.insertBefore("X", "Y"), std::out_of_range);

    REQUIRE_THROWS_AS(list.insertAfter("X", "Y"), std::out_of_range);

    list.pushBack("A");

    REQUIRE_THROWS_AS(list.insertBefore("Missing", "B"), std::out_of_range);

    REQUIRE_THROWS_AS(list.insertAfter("Missing", "B"), std::out_of_range);
}

TEST_CASE("DList copying and assignment", "[dlist]") {
    DList list;
    list.pushBack("A");
    list.pushBack("B");

    DList copied(list);
    REQUIRE(copied.toVector() == list.toVector());

    DList assigned;
    assigned.pushBack("Old");
    assigned = list;

    REQUIRE(assigned.toVector() == list.toVector());

    assigned = assigned;
    REQUIRE(assigned.toVector() == list.toVector());

    copied.clear();
    REQUIRE(list.size() == 2);

    DList emptyList;
    DList emptyCopy(emptyList);
    REQUIRE(emptyCopy.empty());

    assigned = emptyList;
    REQUIRE(assigned.empty());
}

// ==================== QUEUE ====================

TEST_CASE("Queue operations and exceptions", "[queue]") {
    Queue queue;

    REQUIRE(queue.empty());
    REQUIRE_THROWS_AS(queue.pop(), std::out_of_range);
    REQUIRE_THROWS_AS(queue.front(), std::out_of_range);
    REQUIRE_THROWS_AS(queue.back(), std::out_of_range);

    queue.push("A");
    REQUIRE(queue.front() == "A");
    REQUIRE(queue.back() == "A");

    queue.push("B");
    queue.push("C");

    REQUIRE(queue.toVector() == std::vector<std::string>{"A", "B", "C"});

    REQUIRE(queue.pop() == "A");
    REQUIRE(queue.pop() == "B");
    REQUIRE(queue.pop() == "C");

    REQUIRE(queue.empty());

    queue.push("Again");
    REQUIRE(queue.front() == "Again");
}

TEST_CASE("Queue copying and assignment", "[queue]") {
    Queue queue;
    queue.push("A");
    queue.push("B");

    Queue copied(queue);
    REQUIRE(copied.toVector() == queue.toVector());

    Queue assigned;
    assigned = queue;

    REQUIRE(assigned.toVector() == queue.toVector());

    assigned = assigned;
    REQUIRE(assigned.toVector() == queue.toVector());

    copied.clear();
    REQUIRE(queue.size() == 2);

    Queue emptyQueue;
    Queue emptyCopy(emptyQueue);
    REQUIRE(emptyCopy.empty());

    assigned = emptyQueue;
    REQUIRE(assigned.empty());
}

// ==================== STACK ====================

TEST_CASE("Stack operations and exceptions", "[stack]") {
    Stack stack;

    REQUIRE(stack.empty());
    REQUIRE_THROWS_AS(stack.pop(), std::out_of_range);
    REQUIRE_THROWS_AS(stack.top(), std::out_of_range);

    stack.push("A");
    REQUIRE(stack.top() == "A");

    stack.push("B");
    stack.push("C");

    REQUIRE(stack.toVector() == std::vector<std::string>{"C", "B", "A"});

    REQUIRE(stack.pop() == "C");
    REQUIRE(stack.pop() == "B");
    REQUIRE(stack.pop() == "A");

    REQUIRE(stack.empty());

    stack.push("Again");
    REQUIRE(stack.top() == "Again");
}

TEST_CASE("Stack copying and assignment", "[stack]") {
    Stack stack;
    stack.push("A");
    stack.push("B");

    Stack copied(stack);
    REQUIRE(copied.toVector() == stack.toVector());

    Stack assigned;
    assigned = stack;

    REQUIRE(assigned.toVector() == stack.toVector());

    assigned = assigned;
    REQUIRE(assigned.toVector() == stack.toVector());

    copied.clear();
    REQUIRE(stack.size() == 2);

    Stack emptyStack;
    Stack emptyCopy(emptyStack);
    REQUIRE(emptyCopy.empty());

    assigned = emptyStack;
    REQUIRE(assigned.empty());
}

// ==================== HASH TABLE ====================

TEST_CASE("HashTable operations and exceptions", "[hash]") {
    HashTable table;

    REQUIRE(table.empty());
    REQUIRE_FALSE(table.contains("missing"));
    REQUIRE_FALSE(table.remove("missing"));

    REQUIRE_THROWS_AS(table.get("missing"), std::out_of_range);

    table.insert("name", "Alex");
    table.insert("city", "Moscow");
    table.insert("language", "C++");

    REQUIRE(table.size() == 3);
    REQUIRE(table.get("name") == "Alex");

    table.insert("name", "Diana");

    REQUIRE(table.get("name") == "Diana");
    REQUIRE(table.size() == 3);

    REQUIRE(table.remove("city"));
    REQUIRE_FALSE(table.contains("city"));
    REQUIRE_FALSE(table.remove("city"));

    REQUIRE(table.size() == 2);

    table.clear();
    REQUIRE(table.empty());

    table.insert("new", "value");
    REQUIRE(table.get("new") == "value");
}

TEST_CASE("HashTable rehashing", "[hash]") {
    HashTable table(2);

    for (int i = 0; i < 30; ++i) {
        std::string key = "key" + std::to_string(i);
        std::string value = "value" + std::to_string(i);

        table.insert(key, value);
    }

    REQUIRE(table.size() == 30);

    for (int i = 0; i < 30; ++i) {
        std::string key = "key" + std::to_string(i);
        std::string value = "value" + std::to_string(i);

        REQUIRE(table.contains(key));
        REQUIRE(table.get(key) == value);
    }
}

TEST_CASE("HashTable copying and assignment", "[hash]") {
    HashTable table;

    table.insert("A", "1");
    table.insert("B", "2");

    HashTable copied(table);
    REQUIRE(copied.get("A") == "1");

    HashTable assigned;
    assigned = table;

    REQUIRE(assigned.size() == 2);

    assigned = assigned;
    REQUIRE(assigned.size() == 2);

    copied.insert("C", "3");
    REQUIRE(table.size() == 2);

    HashTable emptyTable;
    HashTable emptyCopy(emptyTable);
    REQUIRE(emptyCopy.empty());

    assigned = emptyTable;
    REQUIRE(assigned.empty());
}

// ==================== AVL TREE ====================

TEST_CASE("AVLTree basic operations", "[avl]") {
    AVLTree tree;

    REQUIRE(tree.empty());
    REQUIRE(tree.height() == 0);

    tree.insert("B");
    tree.insert("A");
    tree.insert("C");
    tree.insert("D");

    REQUIRE(tree.size() == 4);
    REQUIRE(tree.contains("A"));
    REQUIRE_FALSE(tree.contains("Missing"));

    REQUIRE(tree.toVector() == std::vector<std::string>{"A", "B", "C", "D"});

    REQUIRE_FALSE(tree.remove("Missing"));
    REQUIRE(tree.remove("A"));
    REQUIRE_FALSE(tree.contains("A"));
}

TEST_CASE("AVLTree rotations", "[avl]") {
    SECTION("Left rotation") {
        AVLTree tree;
        tree.insert("A");
        tree.insert("B");
        tree.insert("C");

        REQUIRE(tree.toVector() == std::vector<std::string>{"A", "B", "C"});
        REQUIRE(tree.height() == 2);
    }

    SECTION("Right rotation") {
        AVLTree tree;
        tree.insert("C");
        tree.insert("B");
        tree.insert("A");

        REQUIRE(tree.toVector() == std::vector<std::string>{"A", "B", "C"});
        REQUIRE(tree.height() == 2);
    }

    SECTION("Left-right rotation") {
        AVLTree tree;
        tree.insert("C");
        tree.insert("A");
        tree.insert("B");

        REQUIRE(tree.height() == 2);
    }

    SECTION("Right-left rotation") {
        AVLTree tree;
        tree.insert("A");
        tree.insert("C");
        tree.insert("B");

        REQUIRE(tree.height() == 2);
    }
}

TEST_CASE("AVLTree removal cases", "[avl]") {
    SECTION("Remove leaf") {
        AVLTree tree;
        tree.insert("B");
        tree.insert("A");
        tree.insert("C");

        REQUIRE(tree.remove("A"));
        REQUIRE_FALSE(tree.contains("A"));
    }

    SECTION("Remove node with one child") {
        AVLTree tree;
        tree.insert("B");
        tree.insert("A");

        REQUIRE(tree.remove("B"));
        REQUIRE(tree.toVector() == std::vector<std::string>{"A"});
    }

    SECTION("Remove node with two children") {
        AVLTree tree;
        tree.insert("D");
        tree.insert("B");
        tree.insert("F");
        tree.insert("A");
        tree.insert("C");
        tree.insert("E");
        tree.insert("G");

        REQUIRE(tree.remove("D"));

        REQUIRE(tree.toVector() == std::vector<std::string>{"A", "B", "C", "E", "F", "G"});
    }

    SECTION("Remove all nodes") {
        AVLTree tree;
        tree.insert("A");
        tree.insert("B");
        tree.insert("C");

        REQUIRE(tree.remove("B"));
        REQUIRE(tree.remove("A"));
        REQUIRE(tree.remove("C"));

        REQUIRE(tree.empty());
        REQUIRE(tree.height() == 0);
    }
}

TEST_CASE("AVLTree copying and assignment", "[avl]") {
    AVLTree tree;

    tree.insert("B");
    tree.insert("A");
    tree.insert("C");

    AVLTree copied(tree);
    REQUIRE(copied.toVector() == tree.toVector());

    AVLTree assigned;
    assigned.insert("Old");
    assigned = tree;

    REQUIRE(assigned.toVector() == tree.toVector());

    assigned = assigned;
    REQUIRE(assigned.toVector() == tree.toVector());

    copied.clear();
    REQUIRE(tree.size() == 3);

    AVLTree emptyTree;
    AVLTree emptyCopy(emptyTree);
    REQUIRE(emptyCopy.empty());

    assigned = emptyTree;
    REQUIRE(assigned.empty());

    tree.clear();
    REQUIRE(tree.empty());
}

// ==================== BINARY SERIALIZATION ====================

TEST_CASE("Serialization saves and loads strings", "[serialization]") {
    const std::vector<std::string> original = {"Hello", "World", "Привет", "",
                                               "String with spaces"};

    const std::string filename = "serialization_test.bin";

    Serialization::saveBinary(filename, original);

    const auto loaded = Serialization::loadBinary(filename);

    REQUIRE(loaded == original);

    std::remove(filename.c_str());
}

TEST_CASE("Serialization handles empty data", "[serialization]") {
    const std::vector<std::string> original;
    const std::string filename = "serialization_empty_test.bin";

    Serialization::saveBinary(filename, original);

    const auto loaded = Serialization::loadBinary(filename);

    REQUIRE(loaded.empty());

    std::remove(filename.c_str());
}

TEST_CASE("Serialization throws for missing file", "[serialization]") {
    REQUIRE_THROWS_AS(Serialization::loadBinary("file_that_does_not_exist.bin"),
                      std::runtime_error);
}