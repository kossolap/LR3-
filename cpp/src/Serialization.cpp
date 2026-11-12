
#include "lr3/Serialization.h"

#include <cstdint>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace lr3 {
namespace {

constexpr std::uint64_t maxElements = 1'000'000;
constexpr std::uint64_t maxStringLength = 100'000'000;

} // namespace

void Serialization::saveBinary(const std::string& filename, const std::vector<std::string>& data) {

    if (data.size() > maxElements) {
        throw std::runtime_error("Too many elements to serialize");
    }

    std::ofstream file(filename, std::ios::binary | std::ios::trunc);

    if (!file) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }

    const auto count = static_cast<std::uint64_t>(data.size());
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto& value : data) {
        if (value.size() > maxStringLength) {
            throw std::runtime_error("String is too large to serialize");
        }

        const auto length = static_cast<std::uint64_t>(value.size());

        file.write(reinterpret_cast<const char*>(&length), sizeof(length));

        if (!value.empty()) {
            file.write(value.data(), static_cast<std::streamsize>(value.size()));
        }

        if (!file) {
            throw std::runtime_error("Error while writing binary file");
        }
    }

    if (!file) {
        throw std::runtime_error("Error while writing binary file");
    }
}

std::vector<std::string> Serialization::loadBinary(const std::string& filename) {

    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        throw std::runtime_error("Cannot open file for reading: " + filename);
    }

    std::uint64_t count = 0;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));

    if (!file || count > maxElements ||
        count > static_cast<std::uint64_t>(std::numeric_limits<std::size_t>::max())) {
        throw std::runtime_error("Invalid binary file: incorrect element count");
    }

    std::vector<std::string> data;
    data.reserve(static_cast<std::size_t>(count));

    for (std::uint64_t i = 0; i < count; ++i) {
        std::uint64_t length = 0;
        file.read(reinterpret_cast<char*>(&length), sizeof(length));

        if (!file || length > maxStringLength ||
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

        data.push_back(std::move(value));
    }

    return data;
}

} // namespace lr3
