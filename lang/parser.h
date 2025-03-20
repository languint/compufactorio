#pragma once
#include <memory>
#include <string>
#include <vector>

#include "ast.h"
#include "../util.h"

class Parser {
public:
    std::vector<std::string> tokens;

    explicit Parser(const std::vector<std::string> &tokens_) {
        tokens = tokens_;
        pos = 0;
    }

    std::unique_ptr<ast::nodes::ASTNode> parse() {
        auto root = std::make_unique<ast::nodes::BlockNode>();
        while (pos < tokens.size()) {
            if (auto stmt = parseStatement()) {
                root->statements.push_back(std::move(stmt));
            } else {
                pos++;
            }
        }
        return root;
    }

    // ReSharper disable once CppParameterMayBeConstPtrOrRef
    static std::unique_ptr<ast::nodes::FileTypeNode> getFileType(const ast::nodes::BlockNode *root) {
        for (const auto &node: root->statements) {
            if (const auto fileTypeNode = dynamic_cast<ast::nodes::FileTypeNode *>(node.get())) {
                return std::make_unique<ast::nodes::FileTypeNode>(*fileTypeNode);
            }
        }
        return nullptr;
    }

private:
    size_t pos;

    bool match(const std::string &token) {
        if (pos < tokens.size() && tokens[pos] == token) {
            pos++;
            return true;
        }
        return false;
    }

    std::unique_ptr<ast::nodes::ASTNode> parseStatement() {
        if (pos < tokens.size() && !tokens[pos].empty() && tokens[pos][0] == '#') {
            const std::string fileTypeToken = tokens[pos].substr(1);
            pos++;
            std::cout << fileTypeToken << std::endl;
            return std::make_unique<ast::nodes::FileTypeNode>(types::stringToFileType(fileTypeToken));
        }

        if (match("const")) {
            return parseVariable(true);
        }
        if (match("let")) {
            return parseVariable(false);
        }
        if (match("fn")) {
            return parseFunction();
        }
        if (match("return")) {
            return parseReturn();
        }

        if (pos < tokens.size() && pos + 1 < tokens.size() && tokens[pos + 1] == "(") {
            const std::string functionName = tokens[pos++];
            return parseFunctionCall(functionName);
        }
        if (pos < tokens.size() - 1) {
            if (const std::string nextOp = tokens[pos + 1]; nextOp == "=" || nextOp == "+=" || nextOp == "-=" ||
                                                            nextOp == "*=" || nextOp == "/=" || nextOp == "+" ||
                                                            nextOp == "-" || nextOp == "*" || nextOp == "/") {
                return parseAssignment();
            }
        }
        return nullptr;
    }


    std::unique_ptr<ast::nodes::ASTNode> parseExpression() {
        if (pos >= tokens.size()) {
            return nullptr;
        }

        std::string left = tokens[pos++];

        if (pos < tokens.size()) {
            if (tokens[pos] == "+" || tokens[pos] == "-" ||
                tokens[pos] == "*" || tokens[pos] == "/") {
                const std::string op = tokens[pos++];

                if (pos < tokens.size()) {
                    std::string right = tokens[pos++];
                    return std::make_unique<ast::nodes::BinaryOperatorNode>(
                        left, right, types::stringToBinaryOperator(op));
                }
            } else if (tokens[pos] == "!=" || tokens[pos] == "==" ||
                       tokens[pos] == ">" || tokens[pos] == "<" ||
                       tokens[pos] == ">=" || tokens[pos] == "<=" ||
                       tokens[pos] == "&&" || tokens[pos] == "||") {
                const std::string op = tokens[pos++];

                if (pos < tokens.size()) {
                    std::string right = tokens[pos++];
                    return std::make_unique<ast::nodes::UnaryOperatorNode>(
                        left, right, types::stringToUnaryOperator(op));
                }
            }
        }

        if (!left.empty() && left[0] >= '0' && left[0] <= '9') {
            return std::make_unique<ast::nodes::LiteralNode>(left);
        }

        return std::make_unique<ast::nodes::IdentifierNode>(left);
    }

    std::unique_ptr<ast::nodes::VariableNode> parseVariable(bool isConst) {
        std::string name = tokens[pos++];

        match("=");

        if (pos < tokens.size() && !tokens[pos].empty() && std::isdigit(tokens[pos][0])) {
            std::string value = tokens[pos++];
            match(";");
            return std::make_unique<ast::nodes::VariableNode>(name, value, isConst);
        }

        const size_t startPos = pos;
        std::string exprStr;

        while (pos < tokens.size() && tokens[pos] != ";") {
            exprStr += tokens[pos++] + " ";
        }

        if (!exprStr.empty() && exprStr.back() == ' ')
            exprStr.pop_back();

        match(";");

        const size_t savedPos = pos;
        pos = startPos;

        auto expr = parseExpression();

        pos = savedPos;

        if (expr) {
            auto varNode = std::make_unique<ast::nodes::VariableNode>(name, exprStr, isConst);
            varNode->expression = std::move(expr);
            return varNode;
        }

        util::log("Failed to parse expression '" + exprStr + "' in variable initialization.", BgYellow);
        return std::make_unique<ast::nodes::VariableNode>(name, "0", isConst);
    }

    std::unique_ptr<ast::nodes::ASTNode> parseAssignment() {
        std::string varName = tokens[pos++];
        const std::string op = tokens[pos++];
        std::string value = tokens[pos++];
        match(";");
        return std::make_unique<ast::nodes::BinaryOperatorNode>(varName, value, types::stringToBinaryOperator(op));
    }

    std::vector<std::string> parseParameters() {
        std::vector<std::string> params;
        while (!match(")")) {
            std::string param;
            while (pos < tokens.size() && tokens[pos] != "," && tokens[pos] != ")") {
                param += tokens[pos++] + " ";
            }
            if (!param.empty() && param.back() == ' ') {
                param.pop_back();
            }
            if (!param.empty()) {
                params.push_back(param);
            }
            match(",");
        }
        return params;
    }

    std::unique_ptr<ast::nodes::FunctionNode> parseFunction() {
        std::string name = tokens[pos++];
        match("(");
        auto params = parseParameters();
        match("{");
        std::vector<std::unique_ptr<ast::nodes::ASTNode> > body;
        while (!match("}")) {
            if (auto stmt = parseStatement()) {
                body.push_back(std::move(stmt));
            } else {
                pos++;
            }
        }
        return std::make_unique<ast::nodes::FunctionNode>(name, std::move(params), std::move(body));
    }

    std::unique_ptr<ast::nodes::FunctionCallNode> parseFunctionCall(const std::string &functionName) {
        match("(");
        std::vector<std::unique_ptr<ast::nodes::ASTNode> > args;

        if (!match(")")) {
            do {
                if (auto arg = parseExpression()) {
                    args.push_back(std::move(arg));
                }
            } while (match(","));

            match(")");
        }

        match(";");

        return std::make_unique<ast::nodes::FunctionCallNode>(functionName, std::move(args));
    }

    std::unique_ptr<ast::nodes::ReturnNode> parseReturn() {
        match("return");

        auto expr = parseExpression();
        match(";");

        return std::make_unique<ast::nodes::ReturnNode>(std::move(expr));
    }

    std::unique_ptr<ast::nodes::FileTypeNode> parseFileType() {
        const std::string value = tokens[pos++];

        return std::make_unique<ast::nodes::FileTypeNode>(types::stringToFileType(value));
    }
};
