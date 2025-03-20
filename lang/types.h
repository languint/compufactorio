#pragma once
#include <string>

namespace types {
    enum class BinaryOperator {
        AddEquals, // +=
        SubEquals, // -=
        MulEquals, // *=
        DivEquals, // /=
        Add, // +
        Sub, // -
        Mul, // *
        Div, // /
        Invalid,
    };

    enum class UnaryOperator {
        NotEquals, // !=
        Equals, // ==
        And, // &&
        Or, // ||
        GreaterThan, // >
        LessThan, // <
        GreaterThanOrEqual, // >=
        LessThanOrEqual, // <=
        Not, // !
        Invalid,
    };

    enum class FileType {
        Control,
        Data,
        Settings,
        Invalid,
    };

    inline BinaryOperator stringToBinaryOperator(const std::string &op) {
        if (op == "+=") return BinaryOperator::AddEquals;
        if (op == "-=") return BinaryOperator::SubEquals;
        if (op == "*=") return BinaryOperator::MulEquals;
        if (op == "/=") return BinaryOperator::DivEquals;
        if (op == "+") return BinaryOperator::Add;
        if (op == "-") return BinaryOperator::Sub;
        if (op == "*") return BinaryOperator::Mul;
        if (op == "/") return BinaryOperator::Div;
        return BinaryOperator::Invalid;
    }

    inline std::string binaryOperatorToString(const BinaryOperator op) {
        if (op == BinaryOperator::AddEquals) return "+=";
        if (op == BinaryOperator::SubEquals) return "-=";
        if (op == BinaryOperator::MulEquals) return "*=";
        if (op == BinaryOperator::DivEquals) return "/=";
        if (op == BinaryOperator::Add) return "+";
        if (op == BinaryOperator::Sub) return "-";
        if (op == BinaryOperator::Mul) return "*";
        if (op == BinaryOperator::Div) return "/";
        return "Invalid";
    }

    inline UnaryOperator stringToUnaryOperator(const std::string &op) {
        if (op == "!=") return UnaryOperator::NotEquals;
        if (op == "==") return UnaryOperator::Equals;
        if (op == "&&") return UnaryOperator::And;
        if (op == "||") return UnaryOperator::Or;
        if (op == ">") return UnaryOperator::GreaterThan;
        if (op == "<") return UnaryOperator::LessThan;
        if (op == ">=") return UnaryOperator::GreaterThanOrEqual;
        if (op == "<=") return UnaryOperator::LessThanOrEqual;
        if (op == "!") return UnaryOperator::Not;
        return UnaryOperator::Invalid;
    }

    inline std::string unaryOperatorToString(const UnaryOperator op) {
        if (op == UnaryOperator::NotEquals) return "!=";
        if (op == UnaryOperator::Equals) return "==";
        if (op == UnaryOperator::And) return "&&";
        if (op == UnaryOperator::Or) return "||";
        if (op == UnaryOperator::GreaterThan) return ">";
        if (op == UnaryOperator::LessThan) return "<";
        if (op == UnaryOperator::GreaterThanOrEqual) return ">=";
        if (op == UnaryOperator::LessThanOrEqual) return "<=";
        if (op == UnaryOperator::Not) return "!";
        return "Invalid";
    }

    inline std::string fileTypeToString(const FileType type) {
        if (type == FileType::Control) return "Control";
        if (type == FileType::Data) return "Data";
        if (type == FileType::Settings) return "Settings";
        return "Invalid";
    }

    inline FileType stringToFileType(const std::string &type) {
        if (type == "control") return FileType::Control;
        if (type == "data") return FileType::Data;
        if (type == "settings") return FileType::Settings;
        return FileType::Invalid;
    }
}
