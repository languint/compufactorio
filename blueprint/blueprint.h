#pragma once
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"
#include "zlib.h"

using json = nlohmann::json;

namespace blueprint {
    inline std::string parseJSON(const std::string &path) {
        std::ifstream file(path);

        return json::parse(file).dump();
    }
}
