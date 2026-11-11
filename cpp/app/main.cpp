#include "lr3/AVLTree.h"

#include <iostream>
#include <string>

int main() {
    lr3::AVLTree tree;

    tree.insert("Orange");
    tree.insert("Apple");
    tree.insert("Banana");
    tree.insert("Grape");
    tree.insert("Cherry");

    std::cout << "AVL tree (in-order):" << std::endl;

    for (const std::string& value : tree.toVector()) {
        std::cout << value << std::endl;
    }

    std::cout << "\nContains Apple: " << (tree.contains("Apple") ? "yes" : "no") << std::endl;

    std::cout << "Contains Mango: " << (tree.contains("Mango") ? "yes" : "no") << std::endl;

    std::cout << "Tree height: " << tree.height() << std::endl;
    std::cout << "Tree size: " << tree.size() << std::endl;

    tree.remove("Banana");

    std::cout << "\nAfter removing Banana:" << std::endl;

    for (const std::string& value : tree.toVector()) {
        std::cout << value << std::endl;
    }

    std::cout << "Tree size: " << tree.size() << std::endl;

    return 0;
}