
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
#include <iterator>
#include <limits>
#include <stdexcept>
#include <string>
#include <vector>

using namespace lr3;

namespace {

void changeByte(const std::string& filename,
                std::streamoff position,
                char value) {
    std::fstream file(
        filename,
        std::ios::binary | std::ios::in | std::ios::out
    );

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

void writeTextFile(const std::string& filename,
                   const std::string& content) {
    std::ofstream file(filename, std::ios::trunc);

    if (!file) {
        throw std::runtime_error("Unable to create test file");
    }

    file << content;

    if (!file) {
        throw std::runtime_error("Unable to write test file");
    }
}

Serialization::Data createFilledData() {
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

    Serialization::Data data;
    data.arrayCapacity = array.capacity();
    data.array = array.toVector();
    data.list = list.toVector();
    data.doubleList = doubleList.toVector();
    data.queue = queue.toVector();
    data.stack = stack.toVector();
    data.hashCapacity = hashTable.capacity();
    data.hashTable = hashTable.toVector();
    data.avlTree = avlTree.toVector();

    return data;
}

void requireEqualData(const Serialization::Data& original,
                      const Serialization::Data& restored) {
    REQUIRE(restored.arrayCapacity == original.arrayCapacity);
    REQUIRE(restored.array == original.array);
    REQUIRE(restored.list == original.list);
    REQUIRE(restored.doubleList == original.doubleList);
    REQUIRE(restored.queue == original.queue);
    REQUIRE(restored.stack == original.stack);
    REQUIRE(restored.hashCapacity == original.hashCapacity);

    auto expectedHash = original.hashTable;
    auto actualHash = restored.hashTable;

    auto byKey = [](const auto& left, const auto& right) {
        return left.first < right.first;
    };

    std::sort(expectedHash.begin(), expectedHash.end(), byKey);
    std::sort(actualHash.begin(), actualHash.end(), byKey);

    REQUIRE(actualHash == expectedHash);
    REQUIRE(restored.avlTree == original.avlTree);
}

void requireStructuresRestored(const Serialization::Data& restored) {
    Array restoredArray(
        static_cast<std::size_t>(restored.arrayCapacity)
    );

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

    for (auto it = restored.stack.rbegin();
         it != restored.stack.rend(); ++it) {
        restoredStack.push(*it);
    }

    HashTable restoredHashTable(
        static_cast<std::size_t>(restored.hashCapacity)
    );

    for (const auto& entry : restored.hashTable) {
        restoredHashTable.insert(entry.first, entry.second);
    }

    AVLTree restoredAVLTree;

    for (const auto& value : restored.avlTree) {
        restoredAVLTree.insert(value);
    }

    REQUIRE(restoredArray.toVector() == restored.array);
    REQUIRE(restoredArray.capacity() == restored.arrayCapacity);
    REQUIRE(restoredList.toVector() == restored.list);
    REQUIRE(restoredDoubleList.toVector() == restored.doubleList);
    REQUIRE(restoredQueue.toVector() == restored.queue);
    REQUIRE(restoredStack.toVector() == restored.stack);

    // Хеш-таблица не обязана сохранять порядок элементов.
    // Сравниваем содержимое после сортировки по ключам.
    auto actualHash = restoredHashTable.toVector();
    auto expectedHash = restored.hashTable;

    auto byKey = [](const auto& left, const auto& right) {
        return left.first < right.first;
    };

    std::sort(actualHash.begin(), actualHash.end(), byKey);
    std::sort(expectedHash.begin(), expectedHash.end(), byKey);

    REQUIRE(actualHash == expectedHash);

    REQUIRE(restoredAVLTree.toVector() == restored.avlTree);
}

} // namespace

// ============================================================
// BINARY SERIALIZATION TESTS
// ============================================================

TEST_CASE("Binary serialization restores all structures",
          "[serialization][binary]") {
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
              [](const auto& left, const auto& right) {
                  return left.first < right.first;
              });

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

    for (auto it = restored.stack.rbegin();
         it != restored.stack.rend(); ++it) {
        restoredStack.push(*it);
    }

    HashTable restoredHashTable(
        static_cast<std::size_t>(restored.hashCapacity)
    );

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

    REQUIRE(
        restoredHashTable.get(std::numeric_limits<int>::min()) == "minimum"
    );

    REQUIRE(
        restoredHashTable.get(std::numeric_limits<int>::max()) == "maximum"
    );

    std::remove(filename.c_str());
}

TEST_CASE("Binary serialization handles empty structures",
          "[serialization][binary]") {
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

TEST_CASE("Binary serialization rejects a missing file",
          "[serialization][binary]") {
    REQUIRE_THROWS_AS(
        Serialization::loadBinaryData("serialization_missing_file.bin"),
        std::runtime_error
    );
}

TEST_CASE("Binary serialization rejects a truncated file",
          "[serialization][binary]") {
    const std::string filename = "serialization_truncated_test.bin";

    {
        std::ofstream file(filename, std::ios::binary);
        file.write("LR3", 3);
    }

    REQUIRE_THROWS_AS(
        Serialization::loadBinaryData(filename),
        std::runtime_error
    );

    std::remove(filename.c_str());
}

TEST_CASE("Binary serialization rejects an invalid signature",
          "[serialization][binary]") {
    const std::string filename = "serialization_invalid_magic_test.bin";

    createValidFile(filename);
    changeByte(filename, 0, 'X');

    REQUIRE_THROWS_AS(
        Serialization::loadBinaryData(filename),
        std::runtime_error
    );

    std::remove(filename.c_str());
}

TEST_CASE("Binary serialization rejects an unsupported version",
          "[serialization][binary]") {
    const std::string filename = "serialization_invalid_version_test.bin";

    createValidFile(filename);
    changeByte(filename, 8, 2);

    REQUIRE_THROWS_AS(
        Serialization::loadBinaryData(filename),
        std::runtime_error
    );

    std::remove(filename.c_str());
}

TEST_CASE("Binary serialization rejects trailing data",
          "[serialization][binary]") {
    Serialization::Data original;
    original.hashCapacity = 11;

    const std::string filename = "serialization_trailing_test.bin";

    Serialization::saveBinary(filename, original);

    {
        std::ofstream file(filename, std::ios::binary | std::ios::app);
        file.put('X');
    }

    REQUIRE_THROWS_AS(
        Serialization::loadBinaryData(filename),
        std::runtime_error
    );

    std::remove(filename.c_str());
}

TEST_CASE("Binary serialization rejects duplicate hash keys",
          "[serialization][binary]") {
    Serialization::Data data;
    data.hashCapacity = 11;
    data.hashTable = {
        {10, "first"},
        {10, "second"}
    };

    const std::string filename = "serialization_duplicate_test.bin";

    REQUIRE_THROWS_AS(
        Serialization::saveBinary(filename, data),
        std::runtime_error
    );

    std::remove(filename.c_str());
}

TEST_CASE("Binary serialization rejects invalid hash capacity",
          "[serialization][binary]") {
    Serialization::Data data;
    data.hashCapacity = 0;

    REQUIRE_THROWS_AS(
        Serialization::saveBinary(
            "serialization_invalid_capacity_test.bin", data
        ),
        std::runtime_error
    );

    std::remove("serialization_invalid_capacity_test.bin");
}

// ============================================================
// JSON SERIALIZATION TESTS
// ============================================================

TEST_CASE("JSON serialization restores all seven structures",
          "[serialization][json]") {
    const auto original = createFilledData();
    const std::string filename = "serialization_all_test.json";

    Serialization::saveText(filename, original);
    const auto restored = Serialization::loadTextData(filename);

    requireEqualData(original, restored);
    requireStructuresRestored(restored);

    REQUIRE(
        restored.hashTable.front().first ==
        std::numeric_limits<int>::min()
    );

    REQUIRE(
        restored.hashTable.back().first ==
        std::numeric_limits<int>::max()
    );

    std::remove(filename.c_str());
}

TEST_CASE("JSON serialization handles empty structures",
          "[serialization][json]") {
    Serialization::Data original;

    const std::string filename = "serialization_empty_test.json";

    Serialization::saveText(filename, original);
    const auto restored = Serialization::loadTextData(filename);

    requireEqualData(original, restored);

    REQUIRE(restored.array.empty());
    REQUIRE(restored.list.empty());
    REQUIRE(restored.doubleList.empty());
    REQUIRE(restored.queue.empty());
    REQUIRE(restored.stack.empty());
    REQUIRE(restored.hashTable.empty());
    REQUIRE(restored.avlTree.empty());

    std::remove(filename.c_str());
}

TEST_CASE("JSON serialization preserves special characters",
          "[serialization][json]") {
    Serialization::Data original;

    original.arrayCapacity = 5;
    original.array = {
        "",
        "hello world",
        "line1\nline2",
        "quote: \"hello\"",
        "backslash: \\"
    };

    original.list = {"Привет", "мир"};
    original.doubleList = {"tab\tvalue", "a/b"};
    original.queue = {"first\nsecond"};
    original.stack = {"", "special: !@#$%^&*()"};
    original.hashCapacity = 11;
    original.hashTable = {
        {-1, "отрицательный"},
        {42, "value\nwith\nlines"}
    };
    original.avlTree = {"alpha", "beta", "gamma"};

    const std::string filename = "serialization_special_test.json";

    Serialization::saveText(filename, original);
    const auto restored = Serialization::loadTextData(filename);

    requireEqualData(original, restored);

    std::remove(filename.c_str());
}

TEST_CASE("JSON serialization preserves capacities",
          "[serialization][json]") {
    Serialization::Data original;

    original.arrayCapacity = 100;
    original.array = {"one", "two"};
    original.hashCapacity = 101;
    original.hashTable = {{5, "five"}};

    const std::string filename = "serialization_capacity_test.json";

    Serialization::saveText(filename, original);
    const auto restored = Serialization::loadTextData(filename);

    REQUIRE(restored.arrayCapacity == 100);
    REQUIRE(restored.array.size() == 2);
    REQUIRE(restored.hashCapacity == 101);
    REQUIRE(restored.hashTable.size() == 1);

    std::remove(filename.c_str());
}

TEST_CASE("JSON serialization rejects a missing file",
          "[serialization][json]") {
    const std::string filename = "serialization_missing_test.json";

    std::remove(filename.c_str());

    REQUIRE_THROWS_AS(
        Serialization::loadTextData(filename),
        std::runtime_error
    );
}

TEST_CASE("JSON serialization rejects an empty file",
          "[serialization][json]") {
    const std::string filename = "serialization_empty_file_test.json";

    writeTextFile(filename, "");

    REQUIRE_THROWS_AS(
        Serialization::loadTextData(filename),
        std::runtime_error
    );

    std::remove(filename.c_str());
}

TEST_CASE("JSON serialization rejects malformed JSON",
          "[serialization][json]") {
    const std::string filename = "serialization_malformed_test.json";

    writeTextFile(filename, "{ this is not valid JSON }");

    REQUIRE_THROWS_AS(
        Serialization::loadTextData(filename),
        std::runtime_error
    );

    std::remove(filename.c_str());
}

TEST_CASE("JSON serialization rejects a truncated document",
          "[serialization][json]") {
    const std::string filename = "serialization_truncated_test.json";

    writeTextFile(filename, R"({"version":1,"array":[)");

    REQUIRE_THROWS_AS(
        Serialization::loadTextData(filename),
        std::runtime_error
    );

    std::remove(filename.c_str());
}

TEST_CASE("JSON serialization rejects an unsupported version",
          "[serialization][json]") {
    const std::string filename = "serialization_version_test.json";

    writeTextFile(filename, R"({"version":2})");

    REQUIRE_THROWS_AS(
        Serialization::loadTextData(filename),
        std::runtime_error
    );

    std::remove(filename.c_str());
}

TEST_CASE("JSON serialization rejects missing fields",
          "[serialization][json]") {
    const std::string filename = "serialization_missing_fields_test.json";

    writeTextFile(filename, R"({"version":1})");

    REQUIRE_THROWS_AS(
        Serialization::loadTextData(filename),
        std::runtime_error
    );

    std::remove(filename.c_str());
}

TEST_CASE("JSON serialization rejects duplicate hash keys",
          "[serialization][json]") {
    Serialization::Data data;
    data.hashCapacity = 11;
    data.hashTable = {
        {10, "first"},
        {10, "second"}
    };

    const std::string filename = "serialization_duplicate_json_test.json";

    REQUIRE_THROWS_AS(
        Serialization::saveText(filename, data),
        std::runtime_error
    );

    std::remove(filename.c_str());
}

TEST_CASE("JSON serialization rejects zero hash capacity",
          "[serialization][json]") {
    Serialization::Data data;
    data.hashCapacity = 0;

    const std::string filename = "serialization_zero_hash_test.json";

    REQUIRE_THROWS_AS(
        Serialization::saveText(filename, data),
        std::runtime_error
    );

    std::remove(filename.c_str());
}

TEST_CASE("JSON serialization rejects insufficient array capacity",
          "[serialization][json]") {
    Serialization::Data data;
    data.arrayCapacity = 1;
    data.array = {"one", "two"};

    const std::string filename = "serialization_small_array_test.json";

    REQUIRE_THROWS_AS(
        Serialization::saveText(filename, data),
        std::runtime_error
    );

    std::remove(filename.c_str());
}

TEST_CASE("JSON serialization rejects insufficient hash capacity",
          "[serialization][json]") {
    Serialization::Data data;
    data.hashCapacity = 1;
    data.hashTable = {
        {1, "one"},
        {2, "two"}
    };

    const std::string filename = "serialization_small_hash_test.json";

    REQUIRE_THROWS_AS(
        Serialization::saveText(filename, data),
        std::runtime_error
    );

    std::remove(filename.c_str());
}

TEST_CASE("JSON serialization rejects invalid field types",
          "[serialization][json]") {
    const std::string filename = "serialization_types_test.json";

    writeTextFile(
        filename,
        R"({"version":1,"arrayCapacity":"not-a-number"})"
    );

    REQUIRE_THROWS_AS(
        Serialization::loadTextData(filename),
        std::runtime_error
    );

    std::remove(filename.c_str());
}

TEST_CASE("JSON serialization rejects invalid loaded capacities",
          "[serialization][json]") {
    const std::string filename = "serialization_loaded_capacity_test.json";

    Serialization::Data data;
    data.arrayCapacity = 2;
    data.array = {"one", "two"};

    Serialization::saveText(filename, data);

    {
        std::ifstream input(filename);
        std::string content(
            (std::istreambuf_iterator<char>(input)),
            std::istreambuf_iterator<char>()
        );

        const std::string oldValue = "\"arrayCapacity\": 2";
        const std::string newValue = "\"arrayCapacity\": 1";

        const auto position = content.find(oldValue);
        REQUIRE(position != std::string::npos);

        content.replace(position, oldValue.size(), newValue);
        writeTextFile(filename, content);
    }

    REQUIRE_THROWS_AS(
        Serialization::loadTextData(filename),
        std::runtime_error
    );

    std::remove(filename.c_str());
}

TEST_CASE("JSON serialization rejects duplicate keys on loading",
          "[serialization][json]") {
    const std::string filename = "serialization_loaded_duplicate_test.json";

    Serialization::Data data;
    data.hashCapacity = 11;
    data.hashTable = {
        {1, "one"},
        {2, "two"}
    };

    Serialization::saveText(filename, data);

    {
        std::ifstream input(filename);
        std::string content(
            (std::istreambuf_iterator<char>(input)),
            std::istreambuf_iterator<char>()
        );

        const std::string oldValue = "\"key\": 2";
        const std::string newValue = "\"key\": 1";

        const auto position = content.find(oldValue);
        REQUIRE(position != std::string::npos);

        content.replace(position, oldValue.size(), newValue);
        writeTextFile(filename, content);
    }

    REQUIRE_THROWS_AS(
        Serialization::loadTextData(filename),
        std::runtime_error
    );

    std::remove(filename.c_str());
}

TEST_CASE("Binary and JSON serialization restore identical data",
          "[serialization][binary][json]") {
    const auto original = createFilledData();

    const std::string binaryFile = "serialization_compare_test.bin";
    const std::string jsonFile = "serialization_compare_test.json";

    Serialization::saveBinary(binaryFile, original);
    Serialization::saveText(jsonFile, original);

    const auto fromBinary =
        Serialization::loadBinaryData(binaryFile);

    const auto fromJson =
        Serialization::loadTextData(jsonFile);

    requireEqualData(fromBinary, fromJson);

    std::remove(binaryFile.c_str());
    std::remove(jsonFile.c_str());
}
