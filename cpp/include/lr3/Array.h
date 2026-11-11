#ifndef LR3_ARRAY_HPP
#define LR3_ARRAY_HPP

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

namespace lr3 {

class Array {
  private:
    std::unique_ptr<std::string[]> data_;
    std::size_t size_;
    std::size_t capacity_;

    void ensureCapacity(std::size_t requiredCapacity);

  public:
    Array();
    explicit Array(std::size_t initialCapacity);

    Array(const Array& other);
    Array(Array&& other) noexcept;

    Array& operator=(const Array& other);
    Array& operator=(Array&& other) noexcept;

    ~Array() = default;

    void pushBack(const std::string& value);
    void insert(std::size_t index, const std::string& value);
    void erase(std::size_t index);

    std::string at(std::size_t index) const;
    void set(std::size_t index, const std::string& value);

    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;

    bool empty() const noexcept;

    void clear() noexcept;

    std::vector<std::string> toVector() const;

    void swap(Array& other) noexcept;
};

} // namespace lr3

#endif