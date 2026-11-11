#ifndef LR3_HASH_TABLE_H
#define LR3_HASH_TABLE_H

#include <cstddef>
#include <string>
#include <utility>
#include <vector>

namespace lr3 {

class HashTable {
  public:
    static constexpr std::size_t defaultCapacity = 8;

    explicit HashTable(std::size_t capacity = defaultCapacity);

    HashTable(const HashTable& other);
    HashTable& operator=(const HashTable& other);
    ~HashTable();

    void insert(const std::string& key, const std::string& value);
    bool remove(const std::string& key);

    bool contains(const std::string& key) const;
    std::string get(const std::string& key) const;

    std::size_t size() const noexcept;
    bool empty() const noexcept;

    void clear() noexcept;

    std::vector<std::pair<std::string, std::string>> toVector() const;

  private:
    struct Node {
        std::string key;
        std::string value;
        Node* next;

        // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
        Node(const std::string& keyValue, const std::string& dataValue);
    };

    std::vector<Node*> buckets_;
    std::size_t size_;

    std::size_t hash(const std::string& key) const;
    void rehash();
};

} // namespace lr3

#endif