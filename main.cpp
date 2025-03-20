#include "toml.h"
#include "util.h"
#include <filesystem>

#include "sfml.h"
#include "lang/ast.h"
#include "lang/parser.h"
#include "lang/transpiler.h"


int main(const int argc, char *argv[]) {
    const std::filesystem::path currentExecutingPath = std::filesystem::current_path();

    if (argc < 2) {
        util::log("You must provide a subcommand, try 'help'.", BgRed);
        return EXIT_FAILURE;
    }

    const std::string command = argv[1];
    std::vector<std::string> flags = {};

    for (int i = 1; i < argc; i++) {
        const std::string arg = argv[i];
        flags.push_back(arg);
    }

    if (command == "build") {
        if (!util::fileExists(currentExecutingPath / "project.toml")) {
            util::log("Failed to locate project.toml in " + currentExecutingPath.string() + "!", BgRed);
            return EXIT_FAILURE;
        }

        const auto table = sfml::loadProjectFile(currentExecutingPath);

        if (!is_directory(currentExecutingPath / "src")) {
            util::log("Failed to locate src directory!", BgRed);
            return EXIT_FAILURE;
        }

        if (table.empty()) {
            util::log("Failed to validate project: project.toml is empty!", BgRed);
            return EXIT_FAILURE;
        }

        if (const bool validProject = sfml::validateProjectInfo(table); !validProject) {
            util::log("Failed to validate project!", BgRed);
            return EXIT_FAILURE;
        }

        if (const bool validProject = sfml::validateProjectCompiler(table); !validProject) {
            util::log("Failed to validate project!", BgRed);
            return EXIT_FAILURE;
        }

        const std::string projectName = table["info"]["name"].value<std::string>().value_or("undefined");
        const std::string projectVersion = table["info"]["version"].value<std::string>().value_or("0.0.0");

        util::log("Building project: " + projectName + "@" + projectVersion);

        const std::vector<std::filesystem::path> files = util::findAllFilesInDirectory(
            currentExecutingPath / "src", ".sfml");

        if (files.empty()) {
            util::log("No input files, exiting.", BgYellow);
            return EXIT_SUCCESS;
        }

        const std::string outputDir = table["compile"]["out-dir"].value<std::string>().value_or("dist");

        util::log("Building " + std::to_string(files.size()) + " file(s) to /" + outputDir + ".");


        for (const auto &file: files) {
            const auto tokens = ast::tokenize(util::loadFile(file));
            auto parser = Parser(tokens);
            const std::unique_ptr<ast::nodes::ASTNode> ast = parser.parse();

            ast->repr();


            const std::filesystem::path outputPath = transpiler::getLuaFilePath(
                file, currentExecutingPath / outputDir);

            transpiler::createLuaFile(outputPath);
        }
    } else if (command == "help") {
        std::cout << "Available commands:" << std::endl;
        std::cout << "build - Builds the current project." << std::endl;
        std::cout << "watch - Builds the current project in watch mode." << std::endl;
        std::cout << "package - Packages the current project." << std::endl;
    }

    return 0;
}
