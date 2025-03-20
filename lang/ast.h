#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <regex>
#include <memory>

#include "types.h"

namespace ast::nodes {
    class ASTNode {
    public:
        virtual ~ASTNode() = default;

        virtual void repr() const = 0;
    };

    class ExpressionNode;
    class IdentifierNode;
    class LiteralNode;
    class BinaryOperatorNode;
    class BlockNode;

    class VariableNode final : public ASTNode {
    public:
        std::string name;
        std::string value;
        bool isConst;
        std::unique_ptr<ASTNode> expression;

        VariableNode(std::string name,
                     std::string value, const bool isConst) : name(std::move(name)),
                                                              value(std::move(value)), isConst(isConst) {
        }

        void repr() const override {
            const std::string out = isConst ? "true" : "false";
            std::cout << "VariableNode(" << name << " = " << value << ", const = " + out + ")";

            if (expression) {
                std::cout << " with expression: ";
                expression->repr();
            } else {
                std::cout << std::endl;
            }
        }
    };

    class FunctionNode final : public ASTNode {
    public:
        std::string name;
        std::vector<std::string> arguments;
        std::vector<std::unique_ptr<ASTNode> > body;

        explicit FunctionNode(std::string name, std::vector<std::string> arguments,
                              std::vector<std::unique_ptr<ASTNode> > body) : name(std::move(name)),
                                                                             arguments(std::move(arguments)),
                                                                             body(std::move(body)) {
        }

        void repr() const override {
            std::cout << "FunctionNode(name=" << name << ", args=[";
            for (const auto &arg: arguments) {
                std::cout << arg << " ";
            }
            std::cout << "], body_size=" << body.size() << ")\n";
            for (const auto &stmt: body) {
                stmt->repr();
            }
        }
    };

    class FunctionCallNode final : public ASTNode {
    public:
        std::string name;
        std::vector<std::unique_ptr<ASTNode> > arguments;

        FunctionCallNode(std::string name, std::vector<std::unique_ptr<ASTNode> > arguments)
            : name(std::move(name)), arguments(std::move(arguments)) {
        }

        void repr() const override {
            std::cout << "FunctionCallNode(name=" << name << ")" << std::endl;
        }
    };

    class ReturnNode final : public ASTNode {
    public:
        std::unique_ptr<ASTNode> expression;

        explicit ReturnNode(std::unique_ptr<ASTNode> expression) : expression(std::move(expression)) {
        }

        void repr() const override {
            std::cout << "ReturnNode()" << std::endl;
        }
    };

    class BinaryOperatorNode final : public ASTNode {
    public:
        std::string fst;
        std::string snd;
        types::BinaryOperator op;

        BinaryOperatorNode(std::string fst, std::string snd,
                           const types::BinaryOperator type) : fst(std::move(fst)), snd(std::move(snd)), op(type) {
        }

        void repr() const override {
            std::cout << "BinaryOperatorNode(" << fst << " " << binaryOperatorToString(op) << " " << snd << ")" <<
                    std::endl;
        }
    };

    class UnaryOperatorNode final : public ASTNode {
    public:
        std::string fst;
        std::string snd;
        types::UnaryOperator op;

        UnaryOperatorNode(std::string fst, std::string snd,
                          const types::UnaryOperator type) : fst(std::move(fst)), snd(std::move(snd)), op(type) {
        }

        void repr() const override {
            std::cout << "UnaryOperatorNode(" << fst << " " << unaryOperatorToString(op) << " " << snd << ")" <<
                    std::endl;
        }
    };

    class BlockNode final : public ASTNode {
    public:
        std::vector<std::unique_ptr<ASTNode> > statements;

        void repr() const override {
            std::cout << "BlockNode(" << statements.size() << ")" << std::endl;
            for (const auto &stmt: statements) {
                stmt->repr();
            }
        }
    };

    class LiteralNode final : public ASTNode {
    public:
        std::string value;

        explicit LiteralNode(std::string value) : value(std::move(value)) {
        }

        void repr() const override {
            std::cout << "LiteralNode(" << value << ")" << std::endl;
        }
    };

    class IdentifierNode final : public ASTNode {
    public:
        std::string value;

        explicit IdentifierNode(std::string value) : value(std::move(value)) {
        }

        void repr() const override {
            std::cout << "IdentifierNode(" << value << ")" << std::endl;
        }
    };

    class ExpressionNode final : public ASTNode {
    public:
        std::vector<std::unique_ptr<ASTNode> > statements;

        explicit ExpressionNode(std::vector<std::unique_ptr<ASTNode> > statements) : statements(std::move(statements)) {
        }

        void repr() const override {
            std::cout << "ExpressionNode(" << statements.size() << ")" << std::endl;
            for (const auto &stmt: statements) {
                stmt->repr();
            }
        }
    };
}

namespace ast {
    inline std::vector<std::string> tokenize(const std::string &code) {
        const std::regex re(R"((\+=|-=|\*=|/=|!=|==|>=|<=|&&|\|\|)|[A-Za-z0-9_:+#]+|[{}(),;<>+\-\*=])");
        const std::sregex_iterator begin(code.begin(), code.end(), re);
        const std::sregex_iterator end;
        std::vector<std::string> tokens;

        for (auto it = begin; it != end; ++it) {
            tokens.push_back(it->str());
        }

        return tokens;
    }
}
