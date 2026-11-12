
#ifndef LR3_LIST_H
#define LR3_LIST_H

#include <cstddef>
#include <string>
#include <vector>

namespace lr3 {

class List {
  private:
    struct Node {
        std::string data;
        Node* next;

        explicit Node(const std::string& value);
    };

    Node* head_;
    Node* tail_;
    std::size_t size_;

  public:
    List();
    List(const List& other);
    List& operator=(const List& other);
    ~List();

    void pushFront(const std::string& value);
    void pushBack(const std::string& value);

    void insertBefore(const std::string& target,
                      const std::string& value); // NOLINT(bugprone-easily-swappable-parameters)
    void insertAfter(const std::string& target,
                     const std::string& value); // NOLINT(bugprone-easily-swappable-parameters)

    void popFront();
    void popBack();

    bool remove(const std::string& value);

    bool contains(const std::string& value) const;

    std::string front() const;
    std::string back() const;

    std::size_t size() const noexcept;
    bool empty() const noexcept;

    void clear() noexcept;

    std::vector<std::string> toVector() const;
};

} // namespace lr3

#endif
