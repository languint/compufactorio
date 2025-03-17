#pragma once
#include "ast.h"
#include <unordered_map>
#include <string>
#include <iostream>

class TypeChecker {
public:
    bool check(ASTNode *node, bool isTest) {
        if (!node) return true;
        if (const auto *assignNode = dynamic_cast<AssignmentNode *>(node)) {
            return checkAssignment(assignNode, isTest);
        }
        if (const auto *memsetNode = dynamic_cast<MemsetNode *>(node)) {
            return checkMemset(memsetNode, isTest);
        }
        if (auto *funcNode = dynamic_cast<FunctionNode *>(node)) {
            return checkFunction(funcNode, isTest);
        }
        if (const auto *eventNode = dynamic_cast<EventRegisterNode *>(node)) {
            return checkEventRegister(eventNode, isTest);
        }
        if (const auto *blockNode = dynamic_cast<BlockNode *>(node)) {
            return checkBlock(blockNode, isTest);
        }
        if (const auto *inputNode = dynamic_cast<InputNode *>(node)) {
            return checkInput(inputNode, isTest);
        }
        if (const auto *outputNode = dynamic_cast<OutputNode *>(node)) {
            return checkOutput(outputNode, isTest);
        }
        std::cerr << "Unknown node type in type checker!\n";
        node->print();
        return true;
    }

private:
    std::unordered_map<std::string, std::string> symbolTable;
    std::unordered_map<std::string, std::string> constantsTable;

    bool checkMemset(const MemsetNode *node, const bool isTest) {
        if (symbolTable.contains(node->name)) {
            if (!isTest) std::cerr << "Error: Variable '" << node->name << "' already declared.\n";
            return true;
        }

        symbolTable[node->name] = node->type;
        if (node->is_const) {
            constantsTable[node->name] = node->name;
        }

        return false;
    }

    bool checkFunction(FunctionNode *node, const bool isTest) {
        std::unordered_map<std::string, std::string> localSymbols;
        for (const auto &param: node->params) {
            if (const auto spacePos = param.find(" "); spacePos != std::string::npos) {
                const std::string type = param.substr(0, spacePos);
                std::string name = param.substr(spacePos + 1);
                localSymbols[name] = type;
            } else {
                if (!isTest) std::cerr << "Error: Parameter '" << param << "' is not well-formed.\n";
                return true;
            }
        }

        for (const auto &[fst, snd]: localSymbols) {
            symbolTable[fst] = snd;
        }

        bool failed = false;
        for (const auto &stmt: node->body) {
            failed = check(stmt.get(), isTest);
        }

        symbolTable[node->name] = node->name;

        return failed;
    }

    bool checkAssignment(const AssignmentNode *node, const bool isTest) const {
        if (!symbolTable.contains(node->var)) {
            if (!isTest) std::cerr << "Error: Variable '" << node->var << "' not declared before assignment.\n";
            return true;
        }
        if (symbolTable.contains(node->var) && constantsTable.contains(node->var)) {
            if (!isTest) std::cerr << "Error: Variable '" << node->var << "' is constant.\n";
            return true;
        }

        return false;
    }

    bool checkEventRegister(const EventRegisterNode *node, const bool isTest) const {
        if (!symbolTable.contains(node->function)) {
            if (!isTest) std::cerr << "Error: Function '" << node->function << "' not defined.\n";
            return true;
        }
        return false;
    }

    bool checkBlock(const BlockNode *node, const bool isTest) {
        bool failed = false;
        for (const auto &stmt: node->statements) {
            failed = check(stmt.get(), isTest);
        }
        return failed;
    }

    bool checkInput(const InputNode *node, const bool isTest) {
        if (symbolTable.contains(node->name)) {
            if (!isTest) std::cerr << "Error: Input '" << node->name << "' already declared.\n";
            return true;
        }
        if (node->type != "int") {
            if (!isTest) std::cerr << "Error: Input '" << node->type << "' not an integer.\n";
            return true;
        }
        if (node->wireColor != "WireColor::Red" && node->wireColor != "WireColor::Green") {
            if (!isTest) std::cerr << "Error: Unknown WireColor '" << node->wireColor << ".\n";
            return true;
        }
        symbolTable[node->name] = node->name;
        return false;
    }

    static bool checkOutput(const OutputNode *node, const bool isTest) {
        if (node->type != "int") {
            if (!isTest) std::cerr << "Error: Output '" << node->type << "' not an integer.\n";
            return true;
        }
        if (node->wireColor != "WireColor::Red" && node->wireColor != "WireColor::Green") {
            if (!isTest) std::cerr << "Error: Unknown WireColor '" << node->wireColor << ".\n";
            return true;
        }
        return false;
    }
};
