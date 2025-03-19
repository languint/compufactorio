#pragma once
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <bits/regex.h>

#include "types.h"

namespace ast::nodes {
    class ASTNode {
    public:
        virtual ~ASTNode() = default;

        virtual void repr() const = 0;
    };

    class VariableNode final : public ASTNode {
    public:
        std::string type;
        std::string name;
        std::string value;

        VariableNode(std::string type, std::string name,
                     std::string value) : type(std::move(type)), name(std::move(name)), value(std::move(value)) {
        }

        void repr() const override {
            std::cout << name << " = " << value << std::endl;
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
            std::cout << fst << binaryOperatorToString(op) << snd << std::endl;
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
            std::cout << fst << unaryOperatorToString(op) << snd << std::endl;
        }
    };
}

namespace ast {
    inline std::vector<std::string> tokenize(const std::string &code) {
        const std::regex re(R"((\+=|-=|\*=|/=)|[A-Za-z0-9_:+#]+|[{}(),;<>+\-\*=])");
        const std::sregex_iterator begin(code.begin(), code.end(), re);
        const std::sregex_iterator end;
        std::vector<std::string> tokens;

        for (auto it = begin; it != end; ++it) {
            tokens.push_back(it->str());
        }

        return tokens;
    }
}
