
#include "lr3/AVLTree.h"
#include "lr3/Array.h"
#include "lr3/DList.h"
#include "lr3/HashTable.h"
#include "lr3/List.h"
#include "lr3/Queue.h"
#include "lr3/Serialization.h"
#include "lr3/Stack.h"

#include <cstdio>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

namespace {

template <typename T> void printValues(const std::string& name, const std::vector<T>& values) {
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

}

int main() {
    const std::string filename = "lr3_demo.bin";

    try {
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

        lr3::Serialization::Data data;
        data.arrayCapacity = array.capacity();
        data.array = array.toVector();
        data.list = list.toVector();
        data.doubleList = doubleList.toVector();
        data.queue = queue.toVector();
        data.stack = stack.toVector();
        data.hashCapacity = hashTable.capacity();
        data.hashTable = hashTable.toVector();
        data.avlTree = avlTree.toVector();

        lr3::Serialization::saveBinary(filename, data);
        std::cout << "All structures saved to " << filename << "\n\n";

        const auto restored = lr3::Serialization::loadBinaryData(filename);

        lr3::Array restoredArray(static_cast<std::size_t>(restored.arrayCapacity));
        for (const auto& value : restored.array) {
            restoredArray.pushBack(value);
        }

        lr3::List restoredList;
        for (const auto& value : restored.list) {
            restoredList.pushBack(value);
        }

        lr3::DList restoredDoubleList;
        for (const auto& value : restored.doubleList) {
            restoredDoubleList.pushBack(value);
        }

        lr3::Queue restoredQueue;
        for (const auto& value : restored.queue) {
            restoredQueue.push(value);
        }

        lr3::Stack restoredStack;
        for (auto it = restored.stack.rbegin(); it != restored.stack.rend(); ++it) {
            restoredStack.push(*it);
        }

        lr3::HashTable restoredHashTable(static_cast<std::size_t>(restored.hashCapacity));
        for (const auto& entry : restored.hashTable) {
            restoredHashTable.insert(entry.first, entry.second);
        }

        lr3::AVLTree restoredAVLTree;
        for (const auto& value : restored.avlTree) {
            restoredAVLTree.insert(value);
        }

        std::cout << "Restored structures:\n";

        printValues("Array", restoredArray.toVector());
        printValues("Singly linked list", restoredList.toVector());
        printValues("Doubly linked list", restoredDoubleList.toVector());
        printValues("Queue", restoredQueue.toVector());
        printValues("Stack", restoredStack.toVector());
        printHashTable(restoredHashTable.toVector());
        printValues("AVL tree (in-order)", restoredAVLTree.toVector());

        std::cout << "\nAVL tree contains Apple: "
                  << (restoredAVLTree.contains("Apple") ? "yes" : "no") << '\n';
        std::cout << "AVL tree height: " << restoredAVLTree.height() << '\n';

        std::remove(filename.c_str());
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        std::remove(filename.c_str());
        return 1;
    }
}
