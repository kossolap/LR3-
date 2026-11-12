
#include "lr3/List.h"

#include <stdexcept>

namespace lr3 {

List::Node::Node(const std::string& value) : data(value), next(nullptr) {}

List::List() : head_(nullptr), tail_(nullptr), size_(0) {}

List::List(const List& other) : List() {
    try {
        Node* current = other.head_;

        while (current != nullptr) {
            pushBack(current->data);
            current = current->next;
        }
    } catch (...) {
        clear();
        throw;
    }
}

List& List::operator=(const List& other) {
    if (this == &other) {
        return *this;
    }

    List copy(other);
    clear();

    head_ = copy.head_;
    tail_ = copy.tail_;
    size_ = copy.size_;

    copy.head_ = nullptr;
    copy.tail_ = nullptr;
    copy.size_ = 0;

    return *this;
}

List::~List() {
    clear();
}

void List::pushFront(const std::string& value) {
    Node* newNode = new Node(value); // NOLINT(cppcoreguidelines-owning-memory)

    newNode->next = head_;
    head_ = newNode;

    if (tail_ == nullptr) {
        tail_ = newNode;
    }

    ++size_;
}

void List::pushBack(const std::string& value) {
    Node* newNode = new Node(value); // NOLINT(cppcoreguidelines-owning-memory)

    if (tail_ == nullptr) {
        head_ = newNode;
        tail_ = newNode;
    } else {
        tail_->next = newNode;
        tail_ = newNode;
    }

    ++size_;
}

void List::insertBefore(const std::string& target,
                        const std::string& value) { // NOLINT(bugprone-easily-swappable-parameters)
    if (head_ == nullptr) {
        throw std::out_of_range("Target not found");
    }

    if (head_->data == target) {
        pushFront(value);
        return;
    }

    Node* current = head_;

    while (current->next != nullptr && current->next->data != target) {
        current = current->next;
    }

    if (current->next == nullptr) {
        throw std::out_of_range("Target not found");
    }

    Node* newNode = new Node(value); // NOLINT(cppcoreguidelines-owning-memory)

    newNode->next = current->next;
    current->next = newNode;

    ++size_;
}

void List::insertAfter(const std::string& target,
                       const std::string& value) { // NOLINT(bugprone-easily-swappable-parameters)
    Node* current = head_;

    while (current != nullptr && current->data != target) {
        current = current->next;
    }

    if (current == nullptr) {
        throw std::out_of_range("Target not found");
    }

    Node* newNode = new Node(value); // NOLINT(cppcoreguidelines-owning-memory)

    newNode->next = current->next;
    current->next = newNode;

    if (tail_ == current) {
        tail_ = newNode;
    }

    ++size_;
}

void List::popFront() {
    if (head_ == nullptr) {
        throw std::out_of_range("List is empty");
    }

    Node* oldHead = head_;

    head_ = head_->next;
    delete oldHead; // NOLINT(cppcoreguidelines-owning-memory)

    --size_;

    if (head_ == nullptr) {
        tail_ = nullptr;
    }
}

void List::popBack() {
    if (head_ == nullptr) {
        throw std::out_of_range("List is empty");
    }

    if (head_ == tail_) {
        delete head_; // NOLINT(cppcoreguidelines-owning-memory)
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
        return;
    }

    Node* current = head_;

    while (current->next != tail_) {
        current = current->next;
    }

    delete tail_; // NOLINT(cppcoreguidelines-owning-memory)

    tail_ = current;
    tail_->next = nullptr;

    --size_;
}

bool List::remove(const std::string& value) {
    if (head_ == nullptr) {
        return false;
    }

    if (head_->data == value) {
        popFront();
        return true;
    }

    Node* current = head_;

    while (current->next != nullptr && current->next->data != value) {
        current = current->next;
    }

    if (current->next == nullptr) {
        return false;
    }

    Node* nodeToDelete = current->next;

    current->next = nodeToDelete->next;

    if (nodeToDelete == tail_) {
        tail_ = current;
    }

    delete nodeToDelete; // NOLINT(cppcoreguidelines-owning-memory)
    --size_;

    return true;
}

bool List::contains(const std::string& value) const {
    Node* current = head_;

    while (current != nullptr) {
        if (current->data == value) {
            return true;
        }

        current = current->next;
    }

    return false;
}

std::string List::front() const {
    if (head_ == nullptr) {
        throw std::out_of_range("List is empty");
    }

    return head_->data;
}

std::string List::back() const {
    if (tail_ == nullptr) {
        throw std::out_of_range("List is empty");
    }

    return tail_->data;
}

std::size_t List::size() const noexcept {
    return size_;
}

bool List::empty() const noexcept {
    return size_ == 0;
}

void List::clear() noexcept {
    while (head_ != nullptr) {
        Node* nextNode = head_->next;
        delete head_; // NOLINT(cppcoreguidelines-owning-memory)
        head_ = nextNode;
    }

    tail_ = nullptr;
    size_ = 0;
}

std::vector<std::string> List::toVector() const {
    std::vector<std::string> result;

    Node* current = head_;

    while (current != nullptr) {
        result.push_back(current->data);
        current = current->next;
    }

    return result;
}

} // namespace lr3
