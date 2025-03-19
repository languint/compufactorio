#include <bits/fs_fwd.h>
#include <bits/fs_path.h>

#include "util.h"


int main(const int argc, char *argv[]) {
    const std::filesystem::path currentExecutingPath = std::filesystem::current_path();

    if (argc < 2) {
        util::log("You must provide a subcommand", AnsiColor::BgRed);
        return EXIT_FAILURE;
    }

    const std::string command = argv[1];
    constexpr std::vector<std::string> flags = {};

    for (int i = 1; i < argc; i++) {
        const std::string arg = argv[i];
    }

    if (command == "build") {
        if (!util::fileExists(currentExecutingPath / "info.toml")) {
            util::log("No project found at: " + currentExecutingPath.string() + "!", AnsiColor::BgRed);
        } else {
            util::log("Building project...");
        }
    } else if (command == "help") {
        std::cout << "Available commands:" << std::endl;
        std::cout << "build - Builds the current project." << std::endl;
        std::cout << "watch - Builds the current project in watch mode." << std::endl;
        std::cout << "package - Packages the current project." << std::endl;
    }

    return 0;
}
