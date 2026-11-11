#include "lr3/HashTable.h"

#include <functional>
#include <gsl/pointers>
#include <stdexcept>
#include <utility>

namespace lr3 {

// NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
HashTable::Node::Node(const std::string& keyValue, const std::string& dataValue)
    : key(keyValue), value(dataValue), next(nullptr) {}

HashTable::HashTable(std::size_t capacity)
    : buckets_(capacity == 0 ? defaultCapacity : capacity, nullptr), size_(0) {}

HashTable::HashTable(const HashTable& other) : HashTable(other.buckets_.size()) {
    for (const auto& item : other.toVector()) {
        insert(item.first, item.second);
    }
}

HashTable& HashTable::operator=(const HashTable& other) {
    if (this == &other) {
        return *this;
    }

    HashTable copy(other);

    std::swap(buckets_, copy.buckets_);
    std::swap(size_, copy.size_);

    return *this;
}

HashTable::~HashTable() {
    clear();
}

std::size_t HashTable::hash(const std::string& key) const {
    return std::hash<std::string>{}(key) % buckets_.size();
}

void HashTable::rehash() {
    std::vector<Node*> oldBuckets = std::move(buckets_);

    buckets_.assign(oldBuckets.size() * 2, nullptr);
    size_ = 0;

    for (Node* head : oldBuckets) {
        while (head != nullptr) {
            Node* nextNode = head->next;

            const std::size_t index = hash(head->key);

            head->next = buckets_[index];
            buckets_[index] = head;

            ++size_;
            head = nextNode;
        }
    }
}

void HashTable::insert(const std::string& key, const std::string& value) {
    const std::size_t index = hash(key);

    Node* current = buckets_[index];

    while (current != nullptr) {
        if (current->key == key) {
            current->value = value;
            return;
        }

        current = current->next;
    }

    gsl::owner<Node*> newNode = new Node(key, value);

    newNode->next = buckets_[index];
    buckets_[index] = newNode;

    ++size_;

    if (size_ > buckets_.size() * 3 / 4) {
        rehash();
    }
}

bool HashTable::remove(const std::string& key) {
    const std::size_t index = hash(key);

    Node* current = buckets_[index];
    Node* previous = nullptr;

    while (current != nullptr) {
        if (current->key == key) {
            if (previous == nullptr) {
                buckets_[index] = current->next;
            } else {
                previous->next = current->next;
            }

            // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
            delete current;

            --size_;
            return true;
        }

        previous = current;
        current = current->next;
    }

    return false;
}

bool HashTable::contains(const std::string& key) const {
    const std::size_t index = hash(key);

    Node* current = buckets_[index];

    while (current != nullptr) {
        if (current->key == key) {
            return true;
        }

        current = current->next;
    }

    return false;
}

std::string HashTable::get(const std::string& key) const {
    const std::size_t index = hash(key);

    Node* current = buckets_[index];

    while (current != nullptr) {
        if (current->key == key) {
            return current->value;
        }

        current = current->next;
    }

    throw std::out_of_range("Key not found");
}

std::size_t HashTable::size() const noexcept {
    return size_;
}

bool HashTable::empty() const noexcept {
    return size_ == 0;
}

void HashTable::clear() noexcept {
    for (Node*& head : buckets_) {
        while (head != nullptr) {
            Node* nodeToDelete = head;
            head = head->next;

            // NOLINTNEXTLINE(cppcoreguidelines-owning-memory)
            delete nodeToDelete;
        }
    }

    size_ = 0;
}

std::vector<std::pair<std::string, std::string>> HashTable::toVector() const {
    std::vector<std::pair<std::string, std::string>> result;

    for (Node* head : buckets_) {
        Node* current = head;

        while (current != nullptr) {
            result.emplace_back(current->key, current->value);
            current = current->next;
        }
    }

    return result;
}

} // namespace lr3