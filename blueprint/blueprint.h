#pragma once
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"
#include "zlib.h"
#include "../base64.h"

using json = nlohmann::json;

namespace blueprint {
    inline json loadFile(const std::string &path) {
        std::ifstream file(path);
        return json::parse(file);
    }

    inline std::string compressString(const std::string &input) {
        uLongf compressed_size = compressBound(input.size());
        std::vector<unsigned char> compressed_data(compressed_size);

        const int res = compress2(compressed_data.data(), &compressed_size,
                                  reinterpret_cast<const Bytef *>(input.data()),
                                  input.size(), Z_BEST_COMPRESSION);
        if (res != Z_OK) {
            throw std::runtime_error("Compression failed");
        }

        compressed_data.resize(compressed_size);
        return std::string{compressed_data.begin(), compressed_data.end()};
    }

    inline std::string encode(const json &j) {
        const std::string str = j.dump(4);
        const std::string compressed = compressString(str);

        return "0" + base64::to_base64(compressed);
    }
}
