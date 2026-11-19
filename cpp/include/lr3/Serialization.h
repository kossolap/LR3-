
#pragma once

#include <cstdint>
#include <string>
#include <utility>
#include <vector>

namespace lr3 {

class Serialization {
  public:
    struct Data {
        std::uint64_t arrayCapacity = 0;
        std::vector<std::string> array;

        std::vector<std::string> list;
        std::vector<std::string> doubleList;
        std::vector<std::string> queue;
        std::vector<std::string> stack;

        std::uint64_t hashCapacity = 11;
        std::vector<std::pair<int, std::string>> hashTable;

        std::vector<std::string> avlTree;
    };

    // Бинарная сериализация вектора строк
    static void saveBinary(
        const std::string& filename,
        const std::vector<std::string>& data
    );

    static std::vector<std::string> loadBinary(
        const std::string& filename
    );

    // Бинарная сериализация всех семи структур
    static void saveBinary(
        const std::string& filename,
        const Data& data
    );

    static Data loadBinaryData(
        const std::string& filename
    );

    // Текстовая сериализация всех семи структур в JSON
    static void saveText(
        const std::string& filename,
        const Data& data
    );

    static Data loadTextData(
        const std::string& filename
    );
};

}
