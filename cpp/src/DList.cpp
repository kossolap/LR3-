#include "lr3/DList.h"

#include <stdexcept>

namespace lr3 {

DList::Node::Node(const std::string& value) : data(value), next(nullptr), prev(nullptr) {}

DList::DList() : head_(nullptr), tail_(nullptr), size_(0) {}

DList::DList(const DList& other) : DList() {

    Node* current = other.head_;

    while (current != nullptr) {
        pushBack(current->data);
        current = current->next;
    }
}

DList& DList::operator=(const DList& other) {
    if (this == &other) {
        return *this;
    }

    DList copy(other);
    clear();

    head_ = copy.head_;
    tail_ = copy.tail_;
    size_ = copy.size_;

    copy.head_ = nullptr;
    copy.tail_ = nullptr;
    copy.size_ = 0;

    return *this;
}

DList::~DList() {
    clear();
}

void DList::pushFront(const std::string& value) {
    Node* newNode = new Node(value);

    newNode->next = head_;

    if (head_ != nullptr) {
        head_->prev = newNode;
    } else {
        tail_ = newNode;
    }

    head_ = newNode;
    ++size_;
}

void DList::pushBack(const std::string& value) {
    Node* newNode = new Node(value);

    newNode->prev = tail_;

    if (tail_ != nullptr) {
        tail_->next = newNode;
    } else {
        head_ = newNode;
    }

    tail_ = newNode;
    ++size_;
}

void DList::insertBefore(const std::string& target, const std::string& value) {
    Node* current = head_;

    while (current != nullptr && current->data != target) {
        current = current->next;
    }

    if (current == nullptr) {
        throw std::out_of_range("Target not found");
    }

    if (current == head_) {
        pushFront(value);
        return;
    }

    Node* newNode = new Node(value);

    newNode->prev = current->prev;
    newNode->next = current;

    current->prev->next = newNode;
    current->prev = newNode;

    ++size_;
}

void DList::insertAfter(const std::string& target, const std::string& value) {
    Node* current = head_;

    while (current != nullptr && current->data != target) {
        current = current->next;
    }

    if (current == nullptr) {
        throw std::out_of_range("Target not found");
    }

    if (current == tail_) {
        pushBack(value);
        return;
    }

    Node* newNode = new Node(value);

    newNode->next = current->next;
    newNode->prev = current;

    current->next->prev = newNode;
    current->next = newNode;

    ++size_;
}

void DList::popFront() {
    if (head_ == nullptr) {
        throw std::out_of_range("List is empty");
    }

    Node* oldHead = head_;
    head_ = head_->next;

    if (head_ != nullptr) {
        head_->prev = nullptr;
    } else {
        tail_ = nullptr;
    }

    delete oldHead;
    --size_;
}

void DList::popBack() {
    if (tail_ == nullptr) {
        throw std::out_of_range("List is empty");
    }

    Node* oldTail = tail_;
    tail_ = tail_->prev;

    if (tail_ != nullptr) {
        tail_->next = nullptr;
    } else {
        head_ = nullptr;
    }

    delete oldTail;
    --size_;
}

bool DList::remove(const std::string& value) {
    Node* current = head_;

    while (current != nullptr && current->data != value) {
        current = current->next;
    }

    if (current == nullptr) {
        return false;
    }

    if (current == head_) {
        popFront();
        return true;
    }

    if (current == tail_) {
        popBack();
        return true;
    }

    current->prev->next = current->next;
    current->next->prev = current->prev;

    delete current;
    --size_;

    return true;
}

bool DList::contains(const std::string& value) const {
    Node* current = head_;

    while (current != nullptr) {
        if (current->data == value) {
            return true;
        }

        current = current->next;
    }

    return false;
}

std::string DList::front() const {
    if (head_ == nullptr) {
        throw std::out_of_range("List is empty");
    }

    return head_->data;
}

std::string DList::back() const {
    if (tail_ == nullptr) {
        throw std::out_of_range("List is empty");
    }

    return tail_->data;
}

std::size_t DList::size() const noexcept {
    return size_;
}

bool DList::empty() const noexcept {
    return size_ == 0;
}

void DList::clear() noexcept {
    while (head_ != nullptr) {
        Node* nextNode = head_->next;
        delete head_;
        head_ = nextNode;
    }

    tail_ = nullptr;
    size_ = 0;
}

std::vector<std::string> DList::toVector() const {
    std::vector<std::string> result;

    Node* current = head_;

    while (current != nullptr) {
        result.push_back(current->data);
        current = current->next;
    }

    return result;
}

std::vector<std::string> DList::toReverseVector() const {
    std::vector<std::string> result;

    Node* current = tail_;

    while (current != nullptr) {
        result.push_back(current->data);
        current = current->prev;
    }

    return result;
}

} // namespace lr3