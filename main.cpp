#include <bits/fs_fwd.h>
#include <bits/fs_path.h>

#include "toml.h"
#include "util.h"

bool validateTOML(const toml::table &content) {
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
            util::log("Missing field \"" + field + "\" in info.toml!", BgYellow);
            missingFields.push_back(field);
            valid = false;
        }
    }

    return valid;
}

toml::table loadProjectFile(const std::filesystem::path &currentExecutingPath) {
    const std::filesystem::path filePath = currentExecutingPath / "info.toml";

    if (!util::fileExists(filePath)) {
        util::log("Failed to locate info.toml in " + currentExecutingPath.string() + "!", BgRed);
    }

    const auto fileContent = util::loadFile(filePath);

    const auto table = toml::parse(fileContent);

    return table;
}

int main(const int argc, char *argv[]) {
    const std::filesystem::path currentExecutingPath = std::filesystem::current_path();

    if (argc < 2) {
        util::log("You must provide a subcommand", BgRed);
        return EXIT_FAILURE;
    }

    const std::string command = argv[1];
    std::vector<std::string> flags = {};

    for (int i = 1; i < argc; i++) {
        const std::string arg = argv[i];
        flags.push_back(arg);
    }

    if (command == "build") {
        if (!util::fileExists(currentExecutingPath / "info.toml")) {
            util::log("Failed to locate info.toml in " + currentExecutingPath.string() + "!", BgRed);
            return EXIT_FAILURE;
        }

        util::log("Building project...");
        const auto table = loadProjectFile(currentExecutingPath);

        if (table.empty()) {
            util::log("Failed to load project!", BgRed);
            return EXIT_FAILURE;
        }

        if (const bool validProject = validateTOML(table); !validProject) {
            util::log("Failed to validate project!", BgRed);
            return EXIT_FAILURE;
        }
    } else if (command == "help") {
        std::cout << "Available commands:" << std::endl;
        std::cout << "build - Builds the current project." << std::endl;
        std::cout << "watch - Builds the current project in watch mode." << std::endl;
        std::cout << "package - Packages the current project." << std::endl;
    }

    return 0;
}
