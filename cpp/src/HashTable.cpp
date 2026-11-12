#include "lr3/HashTable.h"

#include <limits>
#include <stdexcept>

namespace lr3 {

bool HashTable::isPrime(std::size_t number) noexcept {
    if (number < 2) {
        return false;
    }

    if (number == 2) {
        return true;
    }

    if (number % 2 == 0) {
        return false;
    }

    for (std::size_t divisor = 3; divisor <= number / divisor; divisor += 2) {
        if (number % divisor == 0) {
            return false;
        }
    }

    return true;
}

std::size_t HashTable::nextPrime(std::size_t number) {
    if (number <= 2) {
        return 2;
    }

    if (number % 2 == 0) {
        ++number;
    }

    while (!isPrime(number)) {
        if (number > std::numeric_limits<std::size_t>::max() - 2) {
            throw std::length_error("HashTable capacity is too large");
        }

        number += 2;
    }

    return number;
}

HashTable::HashTable(std::size_t capacity)
    : table_(nextPrime(capacity == 0 ? defaultCapacity : capacity)), size_(0) {}

std::uint64_t HashTable::foldingHash(int key) noexcept {
    std::int64_t number = key;

    if (number < 0) {
        number = -number;
    }

    std::uint64_t value = static_cast<std::uint64_t>(number);
    std::uint64_t sum = 0;

    while (value > 0) {
        sum += value % 1000;
        value /= 1000;
    }

    return sum;
}

std::size_t HashTable::primaryHash(int key) const noexcept {
    return static_cast<std::size_t>(foldingHash(key) % table_.size());
}

std::size_t HashTable::secondaryHash(int key) const noexcept {
    return 1 + static_cast<std::size_t>(foldingHash(key) % (table_.size() - 1));
}

std::size_t HashTable::findIndex(int key) const noexcept {
    const std::size_t capacityValue = table_.size();

    std::size_t index = primaryHash(key);
    const std::size_t step = secondaryHash(key);

    for (std::size_t attempt = 0; attempt < capacityValue; ++attempt) {
        const Entry& entry = table_[index];

        if (entry.state == State::Empty) {
            return capacityValue;
        }

        if (entry.state == State::Occupied && entry.key == key) {
            return index;
        }

        if (index >= capacityValue - step) {
            index -= capacityValue - step;
        } else {
            index += step;
        }
    }

    return capacityValue;
}

std::size_t HashTable::findInsertionIndex(int key) const noexcept {
    const std::size_t capacityValue = table_.size();

    std::size_t index = primaryHash(key);
    const std::size_t step = secondaryHash(key);

    std::size_t firstDeleted = capacityValue;

    for (std::size_t attempt = 0; attempt < capacityValue; ++attempt) {
        const Entry& entry = table_[index];

        if (entry.state == State::Occupied) {
            if (entry.key == key) {
                return index;
            }
        } else if (entry.state == State::Deleted) {
            if (firstDeleted == capacityValue) {
                firstDeleted = index;
            }
        } else {
            return firstDeleted == capacityValue ? index : firstDeleted;
        }

        if (index >= capacityValue - step) {
            index -= capacityValue - step;
        } else {
            index += step;
        }
    }

    return firstDeleted;
}

void HashTable::insert(int key, const std::string& value) {
    const std::size_t index = findInsertionIndex(key);

    if (index == table_.size()) {
        throw std::overflow_error("HashTable is full");
    }

    Entry& entry = table_[index];

    if (entry.state != State::Occupied) {
        ++size_;
    }

    entry.value = value;
    entry.key = key;
    entry.state = State::Occupied;
}

bool HashTable::remove(int key) {
    const std::size_t index = findIndex(key);

    if (index == table_.size()) {
        return false;
    }

    Entry& entry = table_[index];

    entry.value.clear();
    entry.state = State::Deleted;

    --size_;

    return true;
}

bool HashTable::contains(int key) const {
    return findIndex(key) != table_.size();
}

std::string HashTable::get(int key) const {
    const std::size_t index = findIndex(key);

    if (index == table_.size()) {
        throw std::out_of_range("Key not found");
    }

    return table_[index].value;
}

std::size_t HashTable::size() const noexcept {
    return size_;
}

std::size_t HashTable::capacity() const noexcept {
    return table_.size();
}

bool HashTable::empty() const noexcept {
    return size_ == 0;
}

void HashTable::clear() noexcept {
    for (Entry& entry : table_) {
        entry.value.clear();
        entry.key = 0;
        entry.state = State::Empty;
    }

    size_ = 0;
}

std::vector<std::pair<int, std::string>> HashTable::toVector() const {
    std::vector<std::pair<int, std::string>> result;
    result.reserve(size_);

    for (const Entry& entry : table_) {
        if (entry.state == State::Occupied) {
            result.emplace_back(entry.key, entry.value);
        }
    }

    return result;
}

} // namespace lr3