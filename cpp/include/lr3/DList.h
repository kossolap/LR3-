#ifndef LR3_DLIST_H
#define LR3_DLIST_H

#include <cstddef>
#include <string>
#include <vector>

namespace lr3 {

class DList {
  private:
    struct Node {
        std::string data;
        Node* next;
        Node* prev;

        explicit Node(const std::string& value);
    };

    Node* head_;
    Node* tail_;
    std::size_t size_;

  public:
    DList();
    DList(const DList& other);
    DList& operator=(const DList& other);
    ~DList();

    void pushFront(const std::string& value);
    void pushBack(const std::string& value);

    void insertBefore(const std::string& target, const std::string& value);

    void insertAfter(const std::string& target, const std::string& value);

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
    std::vector<std::string> toReverseVector() const;
};

} // namespace lr3

#endif