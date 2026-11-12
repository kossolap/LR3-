
#include "lr3/Stack.h"

#include <stdexcept>

namespace lr3 {

Stack::Node::Node(const std::string& value) : data(value), next(nullptr) {}

Stack::Stack() : top_(nullptr), size_(0) {}

Stack::Stack(const Stack& other) : Stack() {
    try {
        const std::vector<std::string> values = other.toVector();

        for (auto it = values.rbegin(); it != values.rend(); ++it) {
            push(*it);
        }
    } catch (...) {
        clear();
        throw;
    }
}

Stack& Stack::operator=(const Stack& other) {
    if (this == &other) {
        return *this;
    }

    Stack copy(other);
    clear();

    top_ = copy.top_;
    size_ = copy.size_;

    copy.top_ = nullptr;
    copy.size_ = 0;

    return *this;
}

Stack::~Stack() {
    clear();
}

void Stack::push(const std::string& value) {
    Node* newNode = new Node(value);

    newNode->next = top_;
    top_ = newNode;

    ++size_;
}

std::string Stack::pop() {
    if (top_ == nullptr) {
        throw std::out_of_range("Stack is empty");
    }

    Node* oldTop = top_;
    std::string result = oldTop->data;

    top_ = top_->next;

    delete oldTop;
    --size_;

    return result;
}

std::string Stack::top() const {
    if (top_ == nullptr) {
        throw std::out_of_range("Stack is empty");
    }

    return top_->data;
}

bool Stack::empty() const noexcept {
    return size_ == 0;
}

std::size_t Stack::size() const noexcept {
    return size_;
}

void Stack::clear() noexcept {
    while (top_ != nullptr) {
        Node* nextNode = top_->next;
        delete top_;
        top_ = nextNode;
    }

    size_ = 0;
}

std::vector<std::string> Stack::toVector() const {
    std::vector<std::string> result;
    const Node* current = top_;

    while (current != nullptr) {
        result.push_back(current->data);
        current = current->next;
    }

    return result;
}

} // namespace lr3
