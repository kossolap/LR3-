#include "lr3/Array.h"

#include <stdexcept>
#include <utility>

namespace lr3 {

Array::Array() : data_(nullptr), size_(0), capacity_(0) {}

Array::Array(std::size_t initialCapacity)
    : data_(initialCapacity == 0 ? nullptr : std::make_unique<std::string[]>(initialCapacity)),
      size_(0), capacity_(initialCapacity) {}

Array::Array(const Array& other)
    : data_(other.capacity_ == 0 ? nullptr : std::make_unique<std::string[]>(other.capacity_)),
      size_(other.size_), capacity_(other.capacity_) {

    for (std::size_t i = 0; i < size_; ++i) {
        data_[i] = other.data_[i];
    }
}

Array::Array(Array&& other) noexcept
    : data_(std::move(other.data_)), size_(other.size_), capacity_(other.capacity_) {

    other.size_ = 0;
    other.capacity_ = 0;
}

Array& Array::operator=(const Array& other) {
    if (this == &other) {
        return *this;
    }

    Array copy(other);
    swap(copy);

    return *this;
}

Array& Array::operator=(Array&& other) noexcept {
    if (this == &other) {
        return *this;
    }

    data_ = std::move(other.data_);
    size_ = other.size_;
    capacity_ = other.capacity_;

    other.size_ = 0;
    other.capacity_ = 0;

    return *this;
}

void Array::ensureCapacity(std::size_t requiredCapacity) {
    if (requiredCapacity <= capacity_) {
        return;
    }

    std::size_t newCapacity = capacity_ == 0 ? 1 : capacity_ * 2;

    while (newCapacity < requiredCapacity) {
        newCapacity *= 2;
    }

    auto newData = std::make_unique<std::string[]>(newCapacity);

    for (std::size_t i = 0; i < size_; ++i) {
        newData[i] = std::move(data_[i]);
    }

    data_ = std::move(newData);
    capacity_ = newCapacity;
}

void Array::pushBack(const std::string& value) {
    ensureCapacity(size_ + 1);

    data_[size_] = value;
    ++size_;
}

void Array::insert(std::size_t index, const std::string& value) {
    if (index > size_) {
        throw std::out_of_range("Invalid array index");
    }

    ensureCapacity(size_ + 1);

    for (std::size_t i = size_; i > index; --i) {
        data_[i] = std::move(data_[i - 1]);
    }

    data_[index] = value;
    ++size_;
}

void Array::erase(std::size_t index) {
    if (index >= size_) {
        throw std::out_of_range("Invalid array index");
    }

    for (std::size_t i = index; i + 1 < size_; ++i) {
        data_[i] = std::move(data_[i + 1]);
    }

    data_[size_ - 1].clear();
    --size_;
}

std::string Array::at(std::size_t index) const {
    if (index >= size_) {
        throw std::out_of_range("Invalid array index");
    }

    return data_[index];
}

void Array::set(std::size_t index, const std::string& value) {
    if (index >= size_) {
        throw std::out_of_range("Invalid array index");
    }

    data_[index] = value;
}

std::size_t Array::size() const noexcept {
    return size_;
}

std::size_t Array::capacity() const noexcept {
    return capacity_;
}

bool Array::empty() const noexcept {
    return size_ == 0;
}

void Array::clear() noexcept {
    for (std::size_t i = 0; i < size_; ++i) {
        data_[i].clear();
    }

    size_ = 0;
}

std::vector<std::string> Array::toVector() const {
    std::vector<std::string> result;

    for (std::size_t i = 0; i < size_; ++i) {
        result.push_back(data_[i]);
    }

    return result;
}

void Array::swap(Array& other) noexcept {
    data_.swap(other.data_);

    std::swap(size_, other.size_);
    std::swap(capacity_, other.capacity_);
}

} // namespace lr3