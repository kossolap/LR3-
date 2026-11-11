#pragma once

#include <string>
#include <vector>

namespace lr3 {

class Serialization {
  public:
    static void saveBinary(const std::string& filename, const std::vector<std::string>& data);

    static std::vector<std::string> loadBinary(const std::string& filename);
};

} // namespace lr3