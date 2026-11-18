
#include "lr3/AVLTree.h"
#include "lr3/Array.h"
#include "lr3/DList.h"
#include "lr3/HashTable.h"
#include "lr3/List.h"
#include "lr3/Queue.h"
#include "lr3/Serialization.h"
#include "lr3/Stack.h"

#include <algorithm>
#include <catch2/catch_test_macros.hpp>
#include <cstdio>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

using namespace lr3;

namespace {

void changeByte(const std::string& filename, std::streamoff position, char value) {
    std::fstream file(filename, std::ios::binary | std::ios::in | std::ios::out);

    if (!file) {
        throw std::runtime_error("Unable to open test file");
    }

    file.seekp(position);
    file.put(value);

    if (!file) {
        throw std::runtime_error("Unable to modify test file");
    }
}

void createValidFile(const std::string& filename) {
    Serialization::Data data;
    data.hashCapacity = 11;
    Serialization::saveBinary(filename, data);
}

} // namespace

TEST_CASE("Binary serialization restores all structures", "[serialization]") {
    Array array(8);
    array.pushBack("array-one");
    array.pushBack("array-two");

    List list;
    list.pushBack("list-one");
    list.pushBack("list-two");

    DList doubleList;
    doubleList.pushBack("double-one");
    doubleList.pushBack("double-two");

    Queue queue;
    queue.push("queue-one");
    queue.push("queue-two");

    Stack stack;
    stack.push("stack-one");
    stack.push("stack-two");

    HashTable hashTable(17);
    hashTable.insert(20, "twenty");
    hashTable.insert(10, "ten");
    hashTable.insert(std::numeric_limits<int>::min(), "minimum");
    hashTable.insert(std::numeric_limits<int>::max(), "maximum");

    AVLTree avlTree;
    avlTree.insert("middle");
    avlTree.insert("first");
    avlTree.insert("last");

    Serialization::Data original;
    original.arrayCapacity = array.capacity();
    original.array = array.toVector();
    original.list = list.toVector();
    original.doubleList = doubleList.toVector();
    original.queue = queue.toVector();
    original.stack = stack.toVector();
    original.hashCapacity = hashTable.capacity();
    original.hashTable = hashTable.toVector();
    original.avlTree = avlTree.toVector();

    const std::string filename = "serialization_all_test.bin";

    Serialization::saveBinary(filename, original);
    const auto restored = Serialization::loadBinaryData(filename);

    REQUIRE(restored.arrayCapacity == original.arrayCapacity);
    REQUIRE(restored.array == original.array);
    REQUIRE(restored.list == original.list);
    REQUIRE(restored.doubleList == original.doubleList);
    REQUIRE(restored.queue == original.queue);
    REQUIRE(restored.stack == original.stack);
    REQUIRE(restored.hashCapacity == original.hashCapacity);

    auto expectedHash = original.hashTable;
    std::sort(expectedHash.begin(), expectedHash.end(),
              [](const auto& left, const auto& right) { return left.first < right.first; });

    REQUIRE(restored.hashTable == expectedHash);
    REQUIRE(restored.avlTree == original.avlTree);

    Array restoredArray(static_cast<std::size_t>(restored.arrayCapacity));
    for (const auto& value : restored.array) {
        restoredArray.pushBack(value);
    }

    List restoredList;
    for (const auto& value : restored.list) {
        restoredList.pushBack(value);
    }

    DList restoredDoubleList;
    for (const auto& value : restored.doubleList) {
        restoredDoubleList.pushBack(value);
    }

    Queue restoredQueue;
    for (const auto& value : restored.queue) {
        restoredQueue.push(value);
    }

    Stack restoredStack;
    for (auto it = restored.stack.rbegin(); it != restored.stack.rend(); ++it) {
        restoredStack.push(*it);
    }

    HashTable restoredHashTable(static_cast<std::size_t>(restored.hashCapacity));
    for (const auto& entry : restored.hashTable) {
        restoredHashTable.insert(entry.first, entry.second);
    }

    AVLTree restoredAVLTree;
    for (const auto& value : restored.avlTree) {
        restoredAVLTree.insert(value);
    }

    REQUIRE(restoredArray.toVector() == array.toVector());
    REQUIRE(restoredArray.capacity() == array.capacity());
    REQUIRE(restoredList.toVector() == list.toVector());
    REQUIRE(restoredDoubleList.toVector() == doubleList.toVector());
    REQUIRE(restoredQueue.toVector() == queue.toVector());
    REQUIRE(restoredStack.toVector() == stack.toVector());
    REQUIRE(restoredHashTable.toVector() == hashTable.toVector());
    REQUIRE(restoredAVLTree.toVector() == avlTree.toVector());

    REQUIRE(restoredHashTable.get(std::numeric_limits<int>::min()) == "minimum");
    REQUIRE(restoredHashTable.get(std::numeric_limits<int>::max()) == "maximum");

    std::remove(filename.c_str());
}

TEST_CASE("Binary serialization handles empty structures", "[serialization]") {
    Serialization::Data original;
    original.hashCapacity = 11;

    const std::string filename = "serialization_empty_all_test.bin";

    Serialization::saveBinary(filename, original);
    const auto restored = Serialization::loadBinaryData(filename);

    REQUIRE(restored.array.empty());
    REQUIRE(restored.list.empty());
    REQUIRE(restored.doubleList.empty());
    REQUIRE(restored.queue.empty());
    REQUIRE(restored.stack.empty());
    REQUIRE(restored.hashTable.empty());
    REQUIRE(restored.avlTree.empty());
    REQUIRE(restored.hashCapacity == 11);

    std::remove(filename.c_str());
}

TEST_CASE("Binary serialization rejects a missing file", "[serialization]") {
    REQUIRE_THROWS_AS(Serialization::loadBinaryData("serialization_missing_file.bin"),
                      std::runtime_error);
}

TEST_CASE("Binary serialization rejects a truncated file", "[serialization]") {
    const std::string filename = "serialization_truncated_test.bin";

    {
        std::ofstream file(filename, std::ios::binary);
        file.write("LR3", 3);
    }

    REQUIRE_THROWS_AS(Serialization::loadBinaryData(filename), std::runtime_error);

    std::remove(filename.c_str());
}

TEST_CASE("Binary serialization rejects an invalid signature", "[serialization]") {
    const std::string filename = "serialization_invalid_magic_test.bin";

    createValidFile(filename);
    changeByte(filename, 0, 'X');

    REQUIRE_THROWS_AS(Serialization::loadBinaryData(filename), std::runtime_error);

    std::remove(filename.c_str());
}

TEST_CASE("Binary serialization rejects an unsupported version", "[serialization]") {
    const std::string filename = "serialization_invalid_version_test.bin";

    createValidFile(filename);
    changeByte(filename, 8, 2);

    REQUIRE_THROWS_AS(Serialization::loadBinaryData(filename), std::runtime_error);

    std::remove(filename.c_str());
}

TEST_CASE("Binary serialization rejects trailing data", "[serialization]") {
    Serialization::Data original;
    original.hashCapacity = 11;

    const std::string filename = "serialization_trailing_test.bin";

    Serialization::saveBinary(filename, original);

    {
        std::ofstream file(filename, std::ios::binary | std::ios::app);
        file.put('X');
    }

    REQUIRE_THROWS_AS(Serialization::loadBinaryData(filename), std::runtime_error);

    std::remove(filename.c_str());
}

TEST_CASE("Binary serialization rejects duplicate hash keys", "[serialization]") {
    Serialization::Data data;
    data.hashCapacity = 11;
    data.hashTable = {{10, "first"}, {10, "second"}};

    const std::string filename = "serialization_duplicate_test.bin";

    REQUIRE_THROWS_AS(Serialization::saveBinary(filename, data), std::runtime_error);

    std::remove(filename.c_str());
}

TEST_CASE("Binary serialization rejects invalid hash capacity", "[serialization]") {
    Serialization::Data data;
    data.hashCapacity = 0;

    REQUIRE_THROWS_AS(Serialization::saveBinary("serialization_invalid_capacity_test.bin", data),
                      std::runtime_error);

    std::remove("serialization_invalid_capacity_test.bin");
}
