#ifndef LR3_STACK_H
#define LR3_STACK_H

#include <cstddef>
#include <string>
#include <vector>

namespace lr3 {

class Stack {
  private:
    struct Node {
        std::string data;
        Node* next;

        explicit Node(const std::string& value);
    };

    Node* top_;
    std::size_t size_;

  public:
    Stack();
    Stack(const Stack& other);
    Stack& operator=(const Stack& other);
    ~Stack();

    void push(const std::string& value);
    std::string pop();

    std::string top() const;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void clear() noexcept;

    std::vector<std::string> toVector() const;
};

} // namespace lr3

#endif