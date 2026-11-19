
#include "lr3/AVLTree.h"
#include "lr3/Array.h"
#include "lr3/DList.h"
#include "lr3/HashTable.h"
#include "lr3/List.h"
#include "lr3/Queue.h"
#include "lr3/Serialization.h"
#include "lr3/Stack.h"

#include <algorithm>
#include <exception>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

namespace {

using Data = lr3::Serialization::Data;

template <typename T>
void printValues(const std::string& name, const std::vector<T>& values) {
    std::cout << name << ": ";

    for (const auto& value : values) {
        std::cout << value << ' ';
    }

    std::cout << '\n';
}

void printHashTable(const std::vector<std::pair<int, std::string>>& entries) {
    std::cout << "Hash table:\n";

    for (const auto& [key, value] : entries) {
        std::cout << "  " << key << " -> " << value << '\n';
    }
}

std::vector<std::pair<int, std::string>> sortedHashEntries(
    std::vector<std::pair<int, std::string>> entries) {
    std::sort(entries.begin(), entries.end());
    return entries;
}

bool equalData(const Data& first, const Data& second) {
    return first.arrayCapacity == second.arrayCapacity &&
           first.array == second.array &&
           first.list == second.list &&
           first.doubleList == second.doubleList &&
           first.queue == second.queue &&
           first.stack == second.stack &&
           first.hashCapacity == second.hashCapacity &&
           sortedHashEntries(first.hashTable) ==
               sortedHashEntries(second.hashTable) &&
           first.avlTree == second.avlTree;
}

Data createDemoData() {
    lr3::Array array(8);
    array.pushBack("Alpha");
    array.pushBack("Beta");

    lr3::List list;
    list.pushBack("First");
    list.pushBack("Second");

    lr3::DList doubleList;
    doubleList.pushBack("Left");
    doubleList.pushBack("Right");

    lr3::Queue queue;
    queue.push("Queue first");
    queue.push("Queue second");

    lr3::Stack stack;
    stack.push("Stack bottom");
    stack.push("Stack top");

    lr3::HashTable hashTable(17);
    hashTable.insert(10, "Ten");
    hashTable.insert(20, "Twenty");

    lr3::AVLTree avlTree;
    avlTree.insert("Orange");
    avlTree.insert("Apple");
    avlTree.insert("Banana");

    Data data;
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

void printRestoredData(const std::string& title, const Data& data) {
    lr3::Array array(static_cast<std::size_t>(data.arrayCapacity));
    for (const auto& value : data.array) {
        array.pushBack(value);
    }

    lr3::List list;
    for (const auto& value : data.list) {
        list.pushBack(value);
    }

    lr3::DList doubleList;
    for (const auto& value : data.doubleList) {
        doubleList.pushBack(value);
    }

    lr3::Queue queue;
    for (const auto& value : data.queue) {
        queue.push(value);
    }

    lr3::Stack stack;
    for (auto it = data.stack.rbegin(); it != data.stack.rend(); ++it) {
        stack.push(*it);
    }

    lr3::HashTable hashTable(static_cast<std::size_t>(data.hashCapacity));
    for (const auto& [key, value] : data.hashTable) {
        hashTable.insert(key, value);
    }

    lr3::AVLTree avlTree;
    for (const auto& value : data.avlTree) {
        avlTree.insert(value);
    }

    std::cout << "\n========== " << title << " ==========\n";

    printValues("1. Array", array.toVector());
    std::cout << "   Array capacity: " << array.capacity() << '\n';

    printValues("2. List", list.toVector());
    printValues("3. DList", doubleList.toVector());
    printValues("4. Queue", queue.toVector());
    printValues("5. Stack", stack.toVector());

    std::cout << "6. ";
    printHashTable(hashTable.toVector());
    std::cout << "   Hash capacity: " << hashTable.capacity() << '\n';

    printValues("7. AVL tree (in-order)", avlTree.toVector());

    std::cout << "   AVL contains Apple: "
              << (avlTree.contains("Apple") ? "yes" : "no") << '\n';
    std::cout << "   AVL height: " << avlTree.height() << '\n';
}

int run() {
    const std::string binaryFilename = "lr3_demo.bin";
    const std::string jsonFilename = "lr3_demo.json";

    std::cout << "Laboratory work 3 - C++17\n";
    std::cout << "Serialization of seven data structures\n";

    const Data original = createDemoData();

    printRestoredData("ORIGINAL DATA", original);

    // Binary serialization.
    lr3::Serialization::saveBinary(binaryFilename, original);
    const Data fromBinary =
        lr3::Serialization::loadBinaryData(binaryFilename);

    std::cout << "\nBinary file saved: " << binaryFilename << '\n';
    printRestoredData("RESTORED FROM BINARY", fromBinary);

    // Text serialization (JSON).
    lr3::Serialization::saveText(jsonFilename, original);
    const Data fromJSON =
        lr3::Serialization::loadTextData(jsonFilename);

    std::cout << "\nJSON file saved: " << jsonFilename << '\n';
    printRestoredData("RESTORED FROM JSON", fromJSON);

    std::cout << "\n========== VERIFICATION ==========\n";

    const bool binaryOK = equalData(original, fromBinary);
    const bool jsonOK = equalData(original, fromJSON);
    const bool formatsEqual = equalData(fromBinary, fromJSON);

    std::cout << "Binary:     "
              << (binaryOK ? "OK" : "FAIL") << '\n';

    std::cout << "JSON:       "
              << (jsonOK ? "OK" : "FAIL") << '\n';

    std::cout << "Comparison: "
              << (formatsEqual ? "OK" : "FAIL") << '\n';

    if (!binaryOK || !jsonOK || !formatsEqual) {
        std::cerr << "\nVerification failed!\n";
        return 1;
    }

    std::cout << "\nAll seven structures restored successfully!\n";
    std::cout << "Files are preserved for inspection:\n";
    std::cout << "  " << binaryFilename << '\n';
    std::cout << "  " << jsonFilename << '\n';

    return 0;
}

} // namespace

int main() {
    try {
        return run();
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }
}
