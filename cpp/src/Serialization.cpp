#include "lr3/Serialization.h"

#include <cstdint>
#include <fstream>
#include <limits>
#include <stdexcept>

namespace lr3 {

void Serialization::saveBinary(const std::string& filename, const std::vector<std::string>& data) {
    std::ofstream file(filename, std::ios::binary);

    if (!file) {
        throw std::runtime_error("Не удалось открыть файл для записи");
    }

    const std::uint64_t count = data.size();

    file.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const std::string& value : data) {
        const std::uint64_t length = value.size();

        file.write(reinterpret_cast<const char*>(&length), sizeof(length));

        file.write(value.data(), static_cast<std::streamsize>(length));
    }

    if (!file) {
        throw std::runtime_error("Ошибка записи бинарного файла");
    }
}

std::vector<std::string> Serialization::loadBinary(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);

    if (!file) {
        throw std::runtime_error("Не удалось открыть файл для чтения");
    }

    std::uint64_t count = 0;

    file.read(reinterpret_cast<char*>(&count), sizeof(count));

    if (!file || count > 1'000'000) {
        throw std::runtime_error("Некорректный бинарный файл");
    }

    std::vector<std::string> data;
    data.reserve(static_cast<std::size_t>(count));

    for (std::uint64_t i = 0; i < count; ++i) {
        std::uint64_t length = 0;

        file.read(reinterpret_cast<char*>(&length), sizeof(length));

        if (!file || length > 100'000'000 ||
            length > static_cast<std::uint64_t>(std::numeric_limits<std::streamsize>::max())) {
            throw std::runtime_error("Некорректная длина строки");
        }

        std::string value(static_cast<std::size_t>(length), '\0');

        if (length > 0) {
            file.read(value.data(), static_cast<std::streamsize>(length));
        }

        if (!file) {
            throw std::runtime_error("Ошибка чтения бинарного файла");
        }

        data.push_back(std::move(value));
    }

    return data;
}

} // namespace lr3