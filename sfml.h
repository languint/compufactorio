#pragma once

#include "toml.h"
#include "util.h"
#include <filesystem>

#include "lang/types.h"

namespace sfml {
    inline bool validateProjectInfo(const toml::table &content) {
        std::vector<std::string> missingFields = {};
        std::vector<std::string> expectedFields = {
            "author", "dependencies", "description", "factorio_version", "name", "title", "version"
        };

        bool valid = true;

        if (!content["info"]) {
            valid = false;
        }

        for (const auto &field: expectedFields) {
            if (!content["info"][field]) {
                util::log("Missing field \"" + field + "\" in project.toml!", BgYellow);
                missingFields.push_back(field);
                valid = false;
            }
        }

        return valid;
    }

    inline bool validateProjectCompiler(const toml::table &content) {
        std::vector<std::string> missingFields = {};
        std::vector<std::string> expectedFields = {
            "out_dir"
        };

        bool valid = true;

        if (!content["compile"]) {
            valid = false;
        }

        for (const auto &field: expectedFields) {
            if (!content["compile"][field]) {
                util::log("Missing field \"" + field + "\" in project.toml!", BgYellow);
                missingFields.push_back(field);
                valid = false;
            }
        }

        return valid;
    }

    inline toml::table loadProjectFile(const std::filesystem::path &currentExecutingPath) {
        const std::filesystem::path filePath = currentExecutingPath / "project.toml";

        if (!util::fileExists(filePath)) {
            util::log("Failed to locate project.toml in " + currentExecutingPath.string() + "!", BgRed);
        }

        const auto fileContent = util::loadFile(filePath);

        const auto table = toml::parse(fileContent);

        return table;
    }

    inline std::string getFolderForFileType(const types::FileType file) {
        if (file == types::FileType::Control) return "control";
        if (file == types::FileType::Data) return "data";
        if (file == types::FileType::Settings) return "settings";
        return "invalid";
    }

    inline std::string getFolderForFileType(const std::string& file) {
        if (file == "/") return "/";
        return "invalid";
    }

    inline void clearDirectory(const std::filesystem::path &path) {
        if (!exists(path)) return;

        for (const auto &entry: std::filesystem::directory_iterator(path))
            remove_all(entry.path());
    }

    inline void createDirectory(const std::filesystem::path &path) {
        if (exists(path)) return;

        create_directory(path);
    }
}
