
#include "lr3/Queue.h"

#include <stdexcept>
#include <utility>

namespace lr3 {

Queue::Node::Node(const std::string& value) : data(value), next(nullptr) {}

Queue::Queue() : front_(nullptr), rear_(nullptr), size_(0) {}

Queue::Queue(const Queue& other) : Queue() {
    try {
        const Node* current = other.front_.get();

        while (current != nullptr) {
            push(current->data);
            current = current->next.get();
        }
    } catch (...) {
        clear();
        throw;
    }
}

Queue& Queue::operator=(const Queue& other) {
    if (this == &other) {
        return *this;
    }

    Queue copy(other);
    clear();

    front_ = std::move(copy.front_);
    rear_ = copy.rear_;
    size_ = copy.size_;

    copy.rear_ = nullptr;
    copy.size_ = 0;

    return *this;
}

Queue::~Queue() {
    clear();
}

void Queue::push(const std::string& value) {
    auto newNode = std::make_unique<Node>(value);
    Node* newRear = newNode.get();

    if (rear_ == nullptr) {
        front_ = std::move(newNode);
    } else {
        rear_->next = std::move(newNode);
    }

    rear_ = newRear;
    ++size_;
}

std::string Queue::pop() {
    if (front_ == nullptr) {
        throw std::out_of_range("Queue is empty");
    }

    std::string result = front_->data;
    front_ = std::move(front_->next);
    --size_;

    if (front_ == nullptr) {
        rear_ = nullptr;
    }

    return result;
}

std::string Queue::front() const {
    if (front_ == nullptr) {
        throw std::out_of_range("Queue is empty");
    }

    return front_->data;
}

std::string Queue::back() const {
    if (rear_ == nullptr) {
        throw std::out_of_range("Queue is empty");
    }

    return rear_->data;
}

bool Queue::empty() const noexcept {
    return size_ == 0;
}

std::size_t Queue::size() const noexcept {
    return size_;
}

void Queue::clear() noexcept {
    while (front_ != nullptr) {
        front_ = std::move(front_->next);
    }

    rear_ = nullptr;
    size_ = 0;
}

std::vector<std::string> Queue::toVector() const {
    std::vector<std::string> result;
    const Node* current = front_.get();

    while (current != nullptr) {
        result.push_back(current->data);
        current = current->next.get();
    }

    return result;
}

} // namespace lr3
