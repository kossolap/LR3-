
#ifndef LR3_AVL_TREE_H
#define LR3_AVL_TREE_H

#include <cstddef>
#include <string>
#include <vector>

namespace lr3 {

class AVLTree {
  private:
    struct Node {
        std::string key;
        int height;
        Node* left;
        Node* right;

        explicit Node(const std::string& value);
    };

    Node* root_;
    std::size_t size_;

    static int nodeHeight(const Node* node) noexcept;
    static int balanceFactor(Node* node);
    static void updateHeight(Node* node);

    static Node* rotateRight(Node* node);
    static Node* rotateLeft(Node* node);
    static Node* balance(Node* node);

    Node* insert(Node* node, const std::string& key);
    Node* remove(Node* node, const std::string& key);
    Node* findMin(Node* node) const;
    Node* removeMin(Node* node);

    bool contains(Node* node, const std::string& key) const;
    void inOrder(Node* node, std::vector<std::string>& result) const;
    void destroy(Node* node) noexcept;
    Node* clone(Node* node);

  public:
    AVLTree();
    AVLTree(const AVLTree& other);
    AVLTree& operator=(const AVLTree& other);
    ~AVLTree();

    void insert(const std::string& key);
    bool remove(const std::string& key);
    bool contains(const std::string& key) const;

    std::size_t size() const noexcept;
    bool empty() const noexcept;
    int height() const;

    void clear() noexcept;
    std::vector<std::string> toVector() const;
};

} // namespace lr3

#endif
