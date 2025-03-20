#pragma once
#include <filesystem>
#include <fstream>

#include "../util.h"

namespace transpiler {
    inline std::filesystem::path getLuaFilePath(const std::filesystem::path &fileName,
                                                const std::filesystem::path &outDirectory) {
        if (!exists(outDirectory)) {
            create_directory(outDirectory);
        }

        std::filesystem::path outFilePath = outDirectory / fileName.stem();
        outFilePath += ".lua";

        return outFilePath;
    }

    inline void createLuaFile(const std::filesystem::path &path) {
        std::ofstream file(path);

        if (!file.good()) {
            util::log("Failed to create lua file: " + path.string(), BgRed);
            exit(1);
        }

        file.close();
    }
}
