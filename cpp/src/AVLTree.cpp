
#include "lr3/AVLTree.h"

#include <algorithm>
#include <utility>

namespace lr3 {

AVLTree::Node::Node(const std::string& value)
    : key(value), height(1), left(nullptr), right(nullptr) {}

AVLTree::AVLTree() : root_(nullptr), size_(0) {}

AVLTree::AVLTree(const AVLTree& other) : root_(clone(other.root_)), size_(other.size_) {}

AVLTree& AVLTree::operator=(const AVLTree& other) {
    if (this == &other) {
        return *this;
    }

    AVLTree copy(other);

    std::swap(root_, copy.root_);
    std::swap(size_, copy.size_);

    return *this;
}

AVLTree::~AVLTree() {
    clear();
}

int AVLTree::nodeHeight(const Node* node) noexcept {
    return node == nullptr ? 0 : node->height;
}

int AVLTree::balanceFactor(Node* node) {
    return node == nullptr ? 0 : nodeHeight(node->right) - nodeHeight(node->left);
}

void AVLTree::updateHeight(Node* node) {
    node->height = 1 + std::max(nodeHeight(node->left), nodeHeight(node->right));
}

AVLTree::Node* AVLTree::rotateRight(Node* node) {
    Node* newRoot = node->left;
    Node* subtree = newRoot->right;

    newRoot->right = node;
    node->left = subtree;

    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

AVLTree::Node* AVLTree::rotateLeft(Node* node) {
    Node* newRoot = node->right;
    Node* subtree = newRoot->left;

    newRoot->left = node;
    node->right = subtree;

    updateHeight(node);
    updateHeight(newRoot);

    return newRoot;
}

AVLTree::Node* AVLTree::balance(Node* node) {
    if (node == nullptr) {
        return nullptr;
    }

    updateHeight(node);

    const int factor = balanceFactor(node);

    if (factor > 1) {
        if (balanceFactor(node->right) < 0) {
            node->right = rotateRight(node->right);
        }

        return rotateLeft(node);
    }

    if (factor < -1) {
        if (balanceFactor(node->left) > 0) {
            node->left = rotateLeft(node->left);
        }

        return rotateRight(node);
    }

    return node;
}

AVLTree::Node* AVLTree::insert(Node* node, const std::string& key) {
    if (node == nullptr) {
        ++size_;
        return new Node(key); // NOLINT(cppcoreguidelines-owning-memory)
    }

    if (key < node->key) {
        node->left = insert(node->left, key);
    } else if (key > node->key) {
        node->right = insert(node->right, key);
    } else {
        return node;
    }

    return balance(node);
}

void AVLTree::insert(const std::string& key) {
    root_ = insert(root_, key);
}

AVLTree::Node* AVLTree::findMin(Node* node) const {
    return node->left == nullptr ? node : findMin(node->left);
}

AVLTree::Node* AVLTree::removeMin(Node* node) {
    if (node->left == nullptr) {
        return node->right;
    }

    node->left = removeMin(node->left);

    return balance(node);
}

AVLTree::Node* AVLTree::remove(Node* node, const std::string& key) {
    if (node == nullptr) {
        return nullptr;
    }

    if (key < node->key) {
        node->left = remove(node->left, key);
    } else if (key > node->key) {
        node->right = remove(node->right, key);
    } else {
        Node* left = node->left;
        Node* right = node->right;

        delete node; // NOLINT(cppcoreguidelines-owning-memory)
        --size_;

        if (right == nullptr) {
            return left;
        }

        Node* minNode = findMin(right);
        minNode->right = removeMin(right);
        minNode->left = left;

        return balance(minNode);
    }

    return balance(node);
}

bool AVLTree::remove(const std::string& key) {
    if (!contains(key)) {
        return false;
    }

    root_ = remove(root_, key);
    return true;
}

bool AVLTree::contains(Node* node, const std::string& key) const {
    if (node == nullptr) {
        return false;
    }

    if (key == node->key) {
        return true;
    }

    if (key < node->key) {
        return contains(node->left, key);
    }

    return contains(node->right, key);
}

bool AVLTree::contains(const std::string& key) const {
    return contains(root_, key);
}

std::size_t AVLTree::size() const noexcept {
    return size_;
}

bool AVLTree::empty() const noexcept {
    return size_ == 0;
}

int AVLTree::height() const {
    return nodeHeight(root_);
}

void AVLTree::inOrder(Node* node, std::vector<std::string>& result) const {
    if (node == nullptr) {
        return;
    }

    inOrder(node->left, result);
    result.push_back(node->key);
    inOrder(node->right, result);
}

std::vector<std::string> AVLTree::toVector() const {
    std::vector<std::string> result;

    inOrder(root_, result);

    return result;
}

void AVLTree::destroy(Node* node) noexcept {
    if (node == nullptr) {
        return;
    }

    destroy(node->left);
    destroy(node->right);

    delete node; // NOLINT(cppcoreguidelines-owning-memory)
}

AVLTree::Node* AVLTree::clone(Node* node) {
    if (node == nullptr) {
        return nullptr;
    }

    Node* copy = new Node(node->key); // NOLINT(cppcoreguidelines-owning-memory)
    copy->height = node->height;

    try {
        copy->left = clone(node->left);
        copy->right = clone(node->right);
    } catch (...) {
        destroy(copy);
        throw;
    }

    return copy;
}

void AVLTree::clear() noexcept {
    destroy(root_);

    root_ = nullptr;
    size_ = 0;
}

} // namespace lr3
