#ifndef LR3_QUEUE_H
#define LR3_QUEUE_H

#include <cstddef>
#include <string>
#include <vector>

namespace lr3 {

class Queue {
  private:
    struct Node {
        std::string data;
        Node* next;

        explicit Node(const std::string& value);
    };

    Node* front_;
    Node* rear_;
    std::size_t size_;

  public:
    Queue();
    Queue(const Queue& other);
    Queue& operator=(const Queue& other);
    ~Queue();

    void push(const std::string& value);
    std::string pop();

    std::string front() const;
    std::string back() const;

    bool empty() const noexcept;
    std::size_t size() const noexcept;

    void clear() noexcept;

    std::vector<std::string> toVector() const;
};

} // namespace lr3

#endif