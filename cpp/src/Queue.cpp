#include "lr3/Queue.h"

#include <stdexcept>

namespace lr3 {

Queue::Node::Node(const std::string& value) : data(value), next(nullptr) {}

Queue::Queue() : front_(nullptr), rear_(nullptr), size_(0) {}

Queue::Queue(const Queue& other) : Queue() {

    Node* current = other.front_;

    while (current != nullptr) {
        push(current->data);
        current = current->next;
    }
}

Queue& Queue::operator=(const Queue& other) {
    if (this == &other) {
        return *this;
    }

    Queue copy(other);
    clear();

    front_ = copy.front_;
    rear_ = copy.rear_;
    size_ = copy.size_;

    copy.front_ = nullptr;
    copy.rear_ = nullptr;
    copy.size_ = 0;

    return *this;
}

Queue::~Queue() {
    clear();
}

void Queue::push(const std::string& value) {
    Node* newNode = new Node(value);

    if (rear_ == nullptr) {
        front_ = newNode;
        rear_ = newNode;
    } else {
        rear_->next = newNode;
        rear_ = newNode;
    }

    ++size_;
}

std::string Queue::pop() {
    if (front_ == nullptr) {
        throw std::out_of_range("Queue is empty");
    }

    Node* oldFront = front_;
    std::string result = oldFront->data;

    front_ = front_->next;

    if (front_ == nullptr) {
        rear_ = nullptr;
    }

    delete oldFront;
    --size_;

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
        Node* nextNode = front_->next;
        delete front_;
        front_ = nextNode;
    }

    rear_ = nullptr;
    size_ = 0;
}

std::vector<std::string> Queue::toVector() const {
    std::vector<std::string> result;

    Node* current = front_;

    while (current != nullptr) {
        result.push_back(current->data);
        current = current->next;
    }

    return result;
}

} // namespace lr3