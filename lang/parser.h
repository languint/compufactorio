#pragma once

#include <memory>
#include <string>
#include <vector>
#include "ast.h"

class Parser {
public:
    std::vector<std::string> tokens;

    explicit Parser(const std::vector<std::string> &tokens_) {
        tokens = tokens_;
        pos = 0;
    }

    std::unique_ptr<ASTNode> parse() {
        auto root = std::make_unique<BlockNode>();
        while (pos < tokens.size()) {
            if (auto stmt = parseStatement()) {
                root->statements.push_back(std::move(stmt));
            } else {
                pos++;
            }
        }
        return root;
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

    std::unique_ptr<ASTNode> parseStatement() {
        if (match("const")) {
            return parseMemset(true);
        }
        if (match("memset")) {
            return parseMemset(false);
        }
        if (match("func")) {
            return parseFunction();
        }
        if (match("events")) {
            return parseEventRegister();
        }
        if (match("input")) {
            return parseInput();
        }
        if (match("output")) {
            return parseOutput();
        }
        if (pos < tokens.size() - 1) {
            if (const std::string nextOp = tokens[pos + 1]; nextOp == "=" || nextOp == "+=" || nextOp == "-=" ||
                                                            nextOp == "*=" || nextOp == "/=") {
                return parseAssignment();
            }
        }
        return nullptr;
    }

    std::unique_ptr<MemsetNode> parseMemset(bool is_const) {
        if (is_const) {
            match("memset");
        }
        match("<");
        std::string type = tokens[pos++];
        match(">");
        std::string name = tokens[pos++];
        match("=");
        int value = std::stoi(tokens[pos++]);
        match(";");
        return std::make_unique<MemsetNode>(type, name, value, is_const);
    }

    std::unique_ptr<FunctionNode> parseFunction() {
        std::string name = tokens[pos++];
        match("(");
        auto params = parseParameters();
        match("{");
        std::vector<std::unique_ptr<ASTNode> > body;
        while (!match("}")) {
            if (auto stmt = parseStatement()) {
                body.push_back(std::move(stmt));
            } else {
                pos++;
            }
        }
        return std::make_unique<FunctionNode>(name, std::move(params), std::move(body));
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

    std::unique_ptr<ASTNode> parseAssignment() {
        std::string varName = tokens[pos++];
        std::string op = tokens[pos++];
        std::string value = tokens[pos++];
        match(";");
        return std::make_unique<AssignmentNode>(varName, op, value);
    }

    std::unique_ptr<EventRegisterNode> parseEventRegister() {
        match(".");
        match("register");
        match("(");
        std::string event = tokens[pos++];
        match(",");
        std::string function = tokens[pos++];
        match(")");
        match(";");
        return std::make_unique<EventRegisterNode>(event, function);
    }

    std::unique_ptr<InputNode> parseInput() {
        match("input");
        match("<");
        std::string type = tokens[pos++];
        match(",");
        std::string wireColor = tokens[pos++];
        match(">");
        std::string name = tokens[pos++];
        match(";");
        return std::make_unique<InputNode>(name, type, wireColor);
    }

    std::unique_ptr<OutputNode> parseOutput() {
        match("output");
        match("<");
        std::string type = tokens[pos++];
        match(",");
        std::string wireColor = tokens[pos++];
        match(">");
        match("(");
        auto params = parseParameters();
        match(";");
        return std::make_unique<OutputNode>(type, wireColor, params[0]);
    }
};
