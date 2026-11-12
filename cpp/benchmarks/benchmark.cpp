#include "lr3/AVLTree.h"
#include "lr3/Array.h"
#include "lr3/DList.h"
#include "lr3/HashTable.h"
#include "lr3/List.h"
#include "lr3/Queue.h"
#include "lr3/Stack.h"

#include <chrono>
#include <iostream>
#include <string>

using Clock = std::chrono::steady_clock;

template <typename Function> void measure(const std::string& name, Function function) {
    const auto start = Clock::now();

    function();

    const auto end = Clock::now();

    const auto elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();

    std::cout << name << ": " << elapsed << " мкс\n";
}

int main() {
    constexpr int N = 10000;

    measure("Array: добавление элементов", [] {
        lr3::Array array;

        for (int i = 0; i < N; ++i) {
            array.pushBack(std::to_string(i));
        }
    });

    measure("List: добавление элементов", [] {
        lr3::List list;

        for (int i = 0; i < N; ++i) {
            list.pushBack(std::to_string(i));
        }
    });

    measure("DList: добавление элементов", [] {
        lr3::DList list;

        for (int i = 0; i < N; ++i) {
            list.pushBack(std::to_string(i));
        }
    });

    measure("Queue: добавление элементов", [] {
        lr3::Queue queue;

        for (int i = 0; i < N; ++i) {
            queue.push(std::to_string(i));
        }
    });

    measure("Stack: добавление элементов", [] {
        lr3::Stack stack;

        for (int i = 0; i < N; ++i) {
            stack.push(std::to_string(i));
        }
    });

    measure("HashTable: добавление элементов", [] {
        lr3::HashTable table(N);

        for (int i = 0; i < N; ++i) {
            table.insert(i, "value");
        }
    });

    measure("AVLTree: добавление элементов", [] {
        lr3::AVLTree tree;

        for (int i = 0; i < N; ++i) {
            tree.insert(std::to_string(i));
        }
    });

    return 0;
}