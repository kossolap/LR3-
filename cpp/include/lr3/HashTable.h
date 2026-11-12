#ifndef LR3_HASH_TABLE_H
#define LR3_HASH_TABLE_H

#include <cstddef>
#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace lr3 {

class HashTable {
  public:
    static constexpr std::size_t defaultCapacity = 11;

    explicit HashTable(std::size_t capacity = defaultCapacity);

    void insert(int key, const std::string& value);
    bool remove(int key);

    bool contains(int key) const;
    std::string get(int key) const;

    std::size_t size() const noexcept;
    std::size_t capacity() const noexcept;
    bool empty() const noexcept;

    void clear() noexcept;

    std::vector<std::pair<int, std::string>> toVector() const;

  private:
    enum class State { Empty, Occupied, Deleted };

    struct Entry {
        int key = 0;
        std::string value;
        State state = State::Empty;
    };

    std::vector<Entry> table_;
    std::size_t size_;

    static std::uint64_t foldingHash(int key) noexcept;
    std::size_t primaryHash(int key) const noexcept;
    std::size_t secondaryHash(int key) const noexcept;

    std::size_t findIndex(int key) const noexcept;
    std::size_t findInsertionIndex(int key) const noexcept;

    static bool isPrime(std::size_t number) noexcept;
    static std::size_t nextPrime(std::size_t number);
};

} // namespace lr3

#endif