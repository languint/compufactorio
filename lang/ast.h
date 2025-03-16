#pragma once

#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>
#include <regex>

class ASTNode {
public:
    virtual ~ASTNode() = default;

    virtual void print() const = 0;
};

class MemsetNode final : public ASTNode {
public:
    std::string type;
    std::string name;
    int value;
    bool is_const;

    MemsetNode(std::string type_, std::string name_, const int value_, const bool is_const_)
        : type(std::move(type_)), name(std::move(name_)), value(value_), is_const(is_const_) {
    }

    void print() const override {
        std::cout << "MemsetNode(type=" << type
                << ", name=" << name
                << ", value=" << value
                << ", is_const=" << is_const << ")\n";
    }
};

class AssignmentNode final : public ASTNode {
public:
    std::string var;
    std::string op;
    std::string value;

    AssignmentNode(std::string var_, std::string op_, std::string value_)
        : var(std::move(var_)), op(std::move(op_)), value(std::move(value_)) {
    }

    void print() const override {
        std::cout << "AssignmentNode(var=" << var
                << ", op=" << op
                << ", value=" << value << ")\n";
    }
};

class FunctionNode final : public ASTNode {
public:
    std::string name;
    std::vector<std::string> params;
    std::vector<std::unique_ptr<ASTNode> > body;

    FunctionNode(std::string name_, std::vector<std::string> params_,
                 std::vector<std::unique_ptr<ASTNode> > body_)
        : name(std::move(name_)), params(std::move(params_)), body(std::move(body_)) {
    }

    void print() const override {
        std::cout << "FunctionNode(name=" << name << ", params=[";
        for (const auto &param: params) {
            std::cout << param << " ";
        }
        std::cout << "], body_size=" << body.size() << ")\n";
        for (const auto &stmt: body) {
            stmt->print();
        }
    }
};

class EventRegisterNode final : public ASTNode {
public:
    std::string event;
    std::string function;

    EventRegisterNode(std::string event_, std::string function_)
        : event(std::move(event_)), function(std::move(function_)) {
    }

    void print() const override {
        std::cout << "EventRegisterNode(event=" << event
                << ", function=" << function << ")\n";
    }
};

class InputNode final : public ASTNode {
public:
    std::string name;
    std::string type;
    std::string wireColor;

    InputNode(std::string name_, std::string type_, std::string wireColor_) : name(std::move(name_)),
                                                                              type(std::move(type_)),
                                                                              wireColor(std::move(wireColor_)) {
    }

    void print() const override {
        std::cout << "InputNode(type=" << type << ", wireColor=" << wireColor << ")\n";
    }
};

class OutputNode final : public ASTNode {
public:
    std::string type;
    std::string wireColor;
    std::string value;

    OutputNode(std::string type_, std::string wireColor_, std::string value_) : type(std::move(type_)),
        wireColor(std::move(wireColor_)), value(std::move(value_)) {
    }

    void print() const override {
        std::cout << "OutputNode(type=" << type << ", wireColor=" << wireColor << ", value=" << value << ")\n";
    }
};

class BlockNode final : public ASTNode {
public:
    std::vector<std::unique_ptr<ASTNode> > statements;

    void print() const override {
        std::cout << "BlockNode with " << statements.size() << " statements:\n";
        for (const auto &stmt: statements) {
            stmt->print();
        }
    }
};

inline std::vector<std::string> tokenize(const std::string &code) {
    const std::regex re(R"((\+=|-=|\*=|/=)|[A-Za-z0-9_:+]+|[{}(),;<>+\-\*=])");
    const std::sregex_iterator begin(code.begin(), code.end(), re);
    const std::sregex_iterator end;
    std::vector<std::string> tokens;
    for (auto it = begin; it != end; ++it) {
        tokens.push_back(it->str());
    }
    return tokens;
}
