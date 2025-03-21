#pragma once
#include <filesystem>
#include <fstream>

#include "../util.h"
#include "ast.h"

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

    inline void writeLuaFile(const std::filesystem::path &path, const std::string &content) {
        std::ofstream file(path);

        if (!file.good()) {
            util::log("Failed to write to lua file: " + path.string(), BgRed);
            exit(1);
        }

        file << content;
        file.close();
    }

    struct Module {
        std::string name;
        std::vector<std::string> imports;
    };

    class Transpiler {
        std::unique_ptr<ast::nodes::ASTNode> ast;
        std::string fileContents;
        int indentLevel = 0;

        std::vector<Module> importedModules;

    public:
        explicit Transpiler(std::unique_ptr<ast::nodes::ASTNode> ast_) {
            ast = std::move(ast_);
        }

        std::string transpile() {
            fileContents = "";

            fileContents += "-- Generated by sfml\n\n";

            if (const auto rootBlock = dynamic_cast<ast::nodes::BlockNode *>(ast.get())) {
                for (const auto &statement: rootBlock->statements) {
                    parseStatement(statement.get());
                    fileContents += "\n";
                }
            }

            return fileContents;
        }

    private:
        [[nodiscard]] std::string getIndentation() const {
            std::string indent;
            for (int i = 0; i < indentLevel; ++i) {
                indent += "\t";
            }
            return indent;
        }

        void parseStatement(const ast::nodes::ASTNode *statement) {
            if (statement == nullptr) return;

            if (const auto *varNode = dynamic_cast<const ast::nodes::VariableNode *>(statement)) {
                createVariable(varNode);
            } else if (const auto *funcNode = dynamic_cast<const ast::nodes::FunctionNode *>(statement)) {
                createFunction(funcNode);
            } else if (const auto *callNode = dynamic_cast<const ast::nodes::FunctionCallNode *>(statement)) {
                createFunctionCall(callNode);
            }
            // Binary operation (including assignment)
            else if (const auto *binaryNode = dynamic_cast<const ast::nodes::BinaryOperatorNode *>(statement)) {
                createBinaryOperation(binaryNode);
            } else if (const auto *returnNode = dynamic_cast<const ast::nodes::ReturnNode *>(statement)) {
                createReturn(returnNode);
            } else if (const auto *importNode = dynamic_cast<const ast::nodes::ImportNode *>(statement)) {
                createImport(importNode);
            } else if (const auto *exportNode = dynamic_cast<const ast::nodes::ExportNode *>(statement)) {
                createExport(exportNode);
            } else if (const auto *blockNode = dynamic_cast<const ast::nodes::BlockNode *>(statement)) {
                for (const auto &stmt: blockNode->statements) {
                    parseStatement(stmt.get());
                    fileContents += "\n";
                }
            } else if (dynamic_cast<const ast::nodes::FileTypeNode *>(statement)) {
            } else {
                fileContents += getIndentation() + "-- Invalid node type!\n";
            }
        }

        void createVariable(const ast::nodes::VariableNode *varNode) {
            const std::string name = varNode->name;
            const std::string value = varNode->value;

            std::string output = getIndentation();
            output += "local ";
            output += name;
            output += " = ";

            if (varNode->expression) {
                output += parseExpression(varNode->expression.get());
            } else {
                output += value;
            }

            fileContents += output;
        }

        void createFunction(const ast::nodes::FunctionNode *funcNode) {
            std::string output = getIndentation();
            output += "function ";
            output += funcNode->name;
            output += "(";

            for (size_t i = 0; i < funcNode->arguments.size(); ++i) {
                if (i > 0) output += ", ";
                output += funcNode->arguments[i];
            }

            output += ")\n";
            fileContents += output;

            indentLevel++;

            for (const auto &stmt: funcNode->body) {
                parseStatement(stmt.get());
                fileContents += "\n";
            }

            indentLevel--;

            fileContents += getIndentation() + "end";
        }

        void createFunctionCall(const ast::nodes::FunctionCallNode* callNode) {
            std::string output = getIndentation();
            output += callNode->name;
            output += "(";

            for (size_t i = 0; i < callNode->arguments.size(); ++i) {
                if (i > 0) output += ", ";
                output += parseExpression(callNode->arguments[i].get());
            }

            output += ")";
            fileContents += output;
        }

        void createBinaryOperation(const ast::nodes::BinaryOperatorNode *binaryNode) {
            std::string output = getIndentation();

            switch (binaryNode->op) {
                case types::BinaryOperator::AddEquals:
                    output += binaryNode->fst + " = " + binaryNode->fst + " + " + binaryNode->snd;
                    break;
                case types::BinaryOperator::SubEquals:
                    output += binaryNode->fst + " = " + binaryNode->fst + " - " + binaryNode->snd;
                    break;
                case types::BinaryOperator::MulEquals:
                    output += binaryNode->fst + " = " + binaryNode->fst + " * " + binaryNode->snd;
                    break;
                case types::BinaryOperator::DivEquals:
                    output += binaryNode->fst + " = " + binaryNode->fst + " / " + binaryNode->snd;
                    break;
                default:
                    output += binaryNode->fst + " " + types::binaryOperatorToString(binaryNode->op) + " " + binaryNode->
                            snd;
                    break;
            }

            fileContents += output;
        }

        void createReturn(const ast::nodes::ReturnNode *returnNode) {
            std::string output = getIndentation();
            output += "return ";

            if (returnNode->expression) {
                output += parseExpression(returnNode->expression.get());
            }

            fileContents += output;
        }

        void createImport(const ast::nodes::ImportNode *importNode) {
            std::string output = getIndentation();
            output += "-- Import from " + importNode->fileName + "\n";
            output += getIndentation() + "local " + importNode->fileName + " = require(\"" + importNode->fileName +
                    "\")\n";

            fileContents += output;
            importedModules.push_back({
                .name = importNode->fileName,
                .imports = 
            });
        }

        void createExport(const ast::nodes::ExportNode *exportNode) {
            // In Lua, exporting is done by returning a table at the end of the module
            std::string output = getIndentation();
            output += "-- Export: ";

            for (size_t i = 0; i < exportNode->arguments.size(); ++i) {
                if (i > 0) output += ", ";
                output += exportNode->arguments[i];
            }

            output += "\nreturn {\n";
            indentLevel++;

            for (size_t i = 0; i < exportNode->arguments.size(); ++i) {
                if (i > 0) output += ",\n";
                output += getIndentation() + exportNode->arguments[i] + " = " + exportNode->arguments[i];
            }

            output += "\n";
            indentLevel--;
            output += getIndentation() + "}";

            fileContents += output;
        }

        static std::string parseExpression(const ast::nodes::ASTNode *expr) {
            if (!expr) return "nil";

            if (const auto *literalNode = dynamic_cast<const ast::nodes::LiteralNode *>(expr)) {
                return literalNode->value;
            }
            if (const auto *identifierNode = dynamic_cast<const ast::nodes::IdentifierNode *>(expr)) {
                return identifierNode->value;
            }
            if (const auto *binaryNode = dynamic_cast<const ast::nodes::BinaryOperatorNode *>(expr)) {
                return binaryNode->fst + " " + binaryOperatorToString(binaryNode->op) + " " + binaryNode->snd;
            }
            if (const auto *unaryNode = dynamic_cast<const ast::nodes::UnaryOperatorNode *>(expr)) {
                switch (unaryNode->op) {
                    case types::UnaryOperator::Equals:
                        return unaryNode->fst + " == " + unaryNode->snd;
                    case types::UnaryOperator::NotEquals:
                        return unaryNode->fst + " ~= " + unaryNode->snd;
                    case types::UnaryOperator::And:
                        return unaryNode->fst + " and " + unaryNode->snd;
                    case types::UnaryOperator::Or:
                        return unaryNode->fst + " or " + unaryNode->snd;
                    case types::UnaryOperator::Not:
                        return "not " + unaryNode->fst;
                    default:
                        return unaryNode->fst + " " + types::unaryOperatorToString(unaryNode->op) + " " + unaryNode->
                               snd;
                }
            } else if (const auto *callNode = dynamic_cast<const ast::nodes::FunctionCallNode *>(expr)) {
                std::string result = callNode->name + "(";
                for (size_t i = 0; i < callNode->arguments.size(); ++i) {
                    if (i > 0) result += ", ";
                    result += parseExpression(callNode->arguments[i].get());
                }
                result += ")";
                return result;
            }

            return "nil";
        }
    };
}
