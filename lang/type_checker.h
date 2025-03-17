#pragma once
#include "ast.h"
#include <unordered_map>
#include <string>
#include <iostream>

class TypeChecker {
public:
    void check(ASTNode *node) {
        if (!node) return;
        if (const auto *memsetNode = dynamic_cast<MemsetNode *>(node)) {
            checkMemset(memsetNode);
        } else if (auto *funcNode = dynamic_cast<FunctionNode *>(node)) {
            checkFunction(funcNode);
        } else if (const auto *assignNode = dynamic_cast<AssignmentNode *>(node)) {
            checkAssignment(assignNode);
        } else if (const auto *eventNode = dynamic_cast<EventRegisterNode *>(node)) {
            checkEventRegister(eventNode);
        } else if (const auto *blockNode = dynamic_cast<BlockNode *>(node)) {
            checkBlock(blockNode);
        } else if (const auto *inputNode = dynamic_cast<InputNode *>(node)) {
            checkInput(inputNode);
        } else if (const auto *outputNode = dynamic_cast<OutputNode *>(node)) {
            checkOutput(outputNode);
        } else {
            std::cerr << "Unknown node type in type checker!\n";
            node->print();
        }
    }

private:
    std::unordered_map<std::string, std::string> symbolTable;

    void checkMemset(const MemsetNode *node) {
        if (symbolTable.contains(node->name)) {
            std::cerr << "Error: Variable '" << node->name << "' already declared.\n";
        }
        symbolTable[node->name] = node->type;
    }

    void checkFunction(FunctionNode *node) {
        std::unordered_map<std::string, std::string> localSymbols;
        for (const auto &param: node->params) {
            if (const auto spacePos = param.find(" "); spacePos != std::string::npos) {
                const std::string type = param.substr(0, spacePos);
                std::string name = param.substr(spacePos + 1);
                localSymbols[name] = type;
            } else {
                std::cerr << "Error: Parameter '" << param << "' is not well-formed.\n";
            }
        }

        for (const auto &[fst, snd]: localSymbols) {
            symbolTable[fst] = snd;
        }

        for (const auto &stmt: node->body) {
            check(stmt.get());
        }
    }

    void checkAssignment(const AssignmentNode *node) const {
        if (!symbolTable.contains(node->var)) {
            std::cerr << "Error: Variable '" << node->var << "' not declared before assignment.\n";
        }
    }

    void checkEventRegister(const EventRegisterNode *node) const {
        if (!symbolTable.contains(node->function)) {
            std::cerr << "Error: Function '" << node->function << "' not defined.\n";
        }
    }

    void checkBlock(const BlockNode *node) {
        for (const auto &stmt: node->statements) {
            check(stmt.get());
        }
    }

    void checkInput(const InputNode *node) {
        if (symbolTable.contains(node->name)) {
            std::cerr << "Error: Input '" << node->name << "' already declared.\n";
        }
        if (node->type != "int") {
            std::cerr << "Error: Input '" << node->type << "' not an integer.\n";
        }
        if (node->wireColor != "WireColor::Red" && node->wireColor != "WireColor::Green") {
            std::cerr << "Error: Unknown WireColor '" << node->wireColor << ".\n";
        }
        symbolTable[node->name] = node->name;
    }

    static void checkOutput(const OutputNode *node) {
        if (node->type != "int") {
            std::cerr << "Error: Output '" << node->type << "' not an integer.\n";
        }
        if (node->wireColor != "WireColor::Red" && node->wireColor != "WireColor::Green") {
            std::cerr << "Error: Unknown WireColor '" << node->wireColor << ".\n";
        }
    }
};
