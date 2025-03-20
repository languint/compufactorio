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

    class Transpiler {
        std::unique_ptr<ast::nodes::ASTNode> ast;
        std::vector<std::string> fileContents;

    public:
        explicit Transpiler(std::unique_ptr<ast::nodes::ASTNode> ast_) {
            ast = std::move(ast_);
        }

    private:
        void createVariable(const std::unique_ptr<ast::nodes::VariableNode> &varNode) {
            const std::string name = varNode->name;
            const std::string value = varNode->value;

            std::string output;
            output += "local ";
            output += name;
            output += " = ";
            output += value;

            fileContents.push_back(output);
        }
    };
}
