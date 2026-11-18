
#include "lr3/Serialization.h"

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

namespace lr3 {
namespace {

constexpr std::uint64_t maxElements = 1'000'000;
constexpr std::uint64_t maxStringLength = 100'000'000;
constexpr std::uint64_t maxCapacity = 1'000'000;
constexpr std::uint32_t formatVersion = 1;

constexpr char magic[] = {'L', 'R', '3', 'B', 'I', 'N', '0', '1'};

void writeU32(std::ofstream& file, std::uint32_t value) {
    char bytes[4];

    for (int i = 0; i < 4; ++i) {
        bytes[i] = static_cast<char>((value >> (i * 8)) & 0xFF);
    }

    file.write(bytes, sizeof(bytes));

    if (!file) {
        throw std::runtime_error("Error while writing binary file");
    }
}

std::uint32_t readU32(std::ifstream& file) {
    unsigned char bytes[4]{};
    file.read(reinterpret_cast<char*>(bytes), sizeof(bytes));

    if (!file) {
        throw std::runtime_error("Invalid binary file: unexpected end of file");
    }

    std::uint32_t value = 0;

    for (int i = 0; i < 4; ++i) {
        value |= static_cast<std::uint32_t>(bytes[i]) << (i * 8);
    }

    return value;
}

void writeU64(std::ofstream& file, std::uint64_t value) {
    char bytes[8];

    for (int i = 0; i < 8; ++i) {
        bytes[i] = static_cast<char>((value >> (i * 8)) & 0xFF);
    }

    file.write(bytes, sizeof(bytes));

    if (!file) {
        throw std::runtime_error("Error while writing binary file");
    }
}

std::uint64_t readU64(std::ifstream& file) {
    unsigned char bytes[8]{};
    file.read(reinterpret_cast<char*>(bytes), sizeof(bytes));

    if (!file) {
        throw std::runtime_error("Invalid binary file: unexpected end of file");
    }

    std::uint64_t value = 0;

    for (int i = 0; i < 8; ++i) {
        value |= static_cast<std::uint64_t>(bytes[i]) << (i * 8);
    }

    return value;
}

void writeString(std::ofstream& file, const std::string& value) {
    if (value.size() > maxStringLength) {
        throw std::runtime_error("String is too large to serialize");
    }

    writeU64(file, static_cast<std::uint64_t>(value.size()));

    if (!value.empty()) {
        file.write(value.data(), static_cast<std::streamsize>(value.size()));
    }

    if (!file) {
        throw std::runtime_error("Error while writing binary file");
    }
}

std::string readString(std::ifstream& file) {
    const std::uint64_t length = readU64(file);

    if (length > maxStringLength ||
        length > static_cast<std::uint64_t>(std::numeric_limits<std::size_t>::max())) {
        throw std::runtime_error("Invalid binary file: incorrect string length");
    }

    std::string value(static_cast<std::size_t>(length), '\0');

    if (length > 0) {
        file.read(value.data(), static_cast<std::streamsize>(length));
    }

    if (!file) {
        throw std::runtime_error("Invalid binary file: unexpected end of file");
    }

    return value;
}

void writeStringVector(std::ofstream& file, const std::vector<std::string>& values) {
    if (values.size() > maxElements) {
        throw std::runtime_error("Too many elements to serialize");
    }

    writeU64(file, static_cast<std::uint64_t>(values.size()));

    for (const auto& value : values) {
        writeString(file, value);
    }
}

std::vector<std::string> readStringVector(std::ifstream& file) {
    const std::uint64_t count = readU64(file);

    if (count > maxElements ||
        count > static_cast<std::uint64_t>(std::numeric_limits<std::size_t>::max())) {
        throw std::runtime_error("Invalid binary file: incorrect element count");
    }

    std::vector<std::string> values;
    values.reserve(static_cast<std::size_t>(count));

    for (std::uint64_t i = 0; i < count; ++i) {
        values.push_back(readString(file));
    }

    return values;
}

std::int64_t decodeSignedKey(std::uint64_t raw) {
    if (raw <= static_cast<std::uint64_t>(std::numeric_limits<std::int64_t>::max())) {
        return static_cast<std::int64_t>(raw);
    }

    return -1 - static_cast<std::int64_t>(~raw);
}

void checkEndOfFile(std::ifstream& file) {
    if (file.peek() != std::char_traits<char>::eof()) {
        throw std::runtime_error("Invalid binary file: unexpected trailing data");
    }

    if (!file.eof()) {
        throw std::runtime_error("Error while reading binary file");
    }
}

}



void Serialization::saveBinary(const std::string& filename, const std::vector<std::string>& data) {
    if (data.size() > maxElements) {
        throw std::runtime_error("Too many elements to serialize");
    }

    std::ofstream file(filename, std::ios::binary | std::ios::trunc);

    if (!file) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    writeU64(file, static_cast<std::uint64_t>(data.size()));

    for (const auto& value : data) {
        writeString(file, value);
    }

    file.flush();

    if (!file) {
        throw std::runtime_error("Error while writing binary file");
    }
}

std::vector<std::string> Serialization::loadBinary(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }

    auto result = readStringVector(file);
    checkEndOfFile(file);

    return result;
}



void Serialization::saveBinary(const std::string& filename, const Data& data) {
    if (data.arrayCapacity > maxCapacity || data.arrayCapacity < data.array.size()) {
        throw std::runtime_error("Invalid array capacity");
    }

    if (data.hashCapacity == 0 || data.hashCapacity > maxCapacity ||
        data.hashTable.size() > data.hashCapacity) {
        throw std::runtime_error("Invalid hash table capacity");
    }

    std::unordered_set<int> keys;

    for (const auto& entry : data.hashTable) {
        if (!keys.insert(entry.first).second) {
            throw std::runtime_error("Duplicate hash table key");
        }
    }

    std::ofstream file(filename, std::ios::binary | std::ios::trunc);

    if (!file) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    file.write(magic, sizeof(magic));

    if (!file) {
        throw std::runtime_error("Error while writing binary header");
    }

    writeU32(file, formatVersion);

    writeU64(file, data.arrayCapacity);
    writeStringVector(file, data.array);

    writeStringVector(file, data.list);
    writeStringVector(file, data.doubleList);
    writeStringVector(file, data.queue);
    writeStringVector(file, data.stack);

    writeU64(file, data.hashCapacity);

    auto sortedEntries = data.hashTable;

    std::sort(sortedEntries.begin(), sortedEntries.end(),
              [](const auto& left, const auto& right) { return left.first < right.first; });

    writeU64(file, static_cast<std::uint64_t>(sortedEntries.size()));

    for (const auto& entry : sortedEntries) {
        writeU64(file, static_cast<std::uint64_t>(static_cast<std::int64_t>(entry.first)));

        writeString(file, entry.second);
    }

    writeStringVector(file, data.avlTree);

    file.flush();

    if (!file) {
        throw std::runtime_error("Error while writing binary file");
    }
}

Serialization::Data Serialization::loadBinaryData(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }

    char fileMagic[sizeof(magic)]{};

    file.read(fileMagic, sizeof(fileMagic));

    if (!file || !std::equal(std::begin(magic), std::end(magic), std::begin(fileMagic))) {
        throw std::runtime_error("Invalid binary file: incorrect signature");
    }

    const std::uint32_t version = readU32(file);

    if (version != formatVersion) {
        throw std::runtime_error("Unsupported binary file version");
    }

    Data data;

    data.arrayCapacity = readU64(file);

    if (data.arrayCapacity > maxCapacity) {
        throw std::runtime_error("Invalid array capacity");
    }

    data.array = readStringVector(file);

    if (data.arrayCapacity < data.array.size()) {
        throw std::runtime_error("Invalid array capacity");
    }

    data.list = readStringVector(file);
    data.doubleList = readStringVector(file);
    data.queue = readStringVector(file);
    data.stack = readStringVector(file);

    data.hashCapacity = readU64(file);

    if (data.hashCapacity == 0 || data.hashCapacity > maxCapacity) {
        throw std::runtime_error("Invalid hash table capacity");
    }

    const std::uint64_t hashCount = readU64(file);

    if (hashCount > maxElements || hashCount > data.hashCapacity) {
        throw std::runtime_error("Invalid hash table element count");
    }

    std::unordered_set<int> keys;
    data.hashTable.reserve(static_cast<std::size_t>(hashCount));

    for (std::uint64_t i = 0; i < hashCount; ++i) {
        const std::int64_t decodedKey = decodeSignedKey(readU64(file));

        if (decodedKey < std::numeric_limits<int>::min() ||
            decodedKey > std::numeric_limits<int>::max()) {
            throw std::runtime_error("Invalid hash table key");
        }

        const int key = static_cast<int>(decodedKey);

        if (!keys.insert(key).second) {
            throw std::runtime_error("Duplicate hash table key");
        }

        data.hashTable.emplace_back(key, readString(file));
    }

    data.avlTree = readStringVector(file);

    checkEndOfFile(file);

    return data;
}

}
