#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

enum AnsiColor {
    Black = 30,
    Red = 31,
    Green = 32,
    Yellow = 33,
    Blue = 34,
    Magenta = 35,
    Cyan = 36,
    White = 37,
    BrightBlack = 90,
    BrightRed = 91,
    BrightGreen = 92,
    BrightYellow = 93,
    BrightBlue = 94,
    BrightMagenta = 95,
    BrightCyan = 96,
    BrightWhite = 97,
    BgBlack = 40,
    BgRed = 41,
    BgGreen = 42,
    BgYellow = 43,
    BgBlue = 44,
    BgMagenta = 45,
    BgCyan = 46,
    BgWhite = 47,
    BgBrightBlack = 100,
    BgBrightRed = 101,
    BgBrightGreen = 102,
    BgBrightYellow = 103,
    BgBrightBlue = 104,
    BgBrightMagenta = 105,
    BgBrightCyan = 106,
    BgBrightWhite = 107,
};

namespace util {
    inline std::string ansiColors(const std::string &string, const std::vector<AnsiColor> &codes) {
        std::string result;

        for (const auto &code: codes) {
            result += "\033[" + std::to_string(code) + "m";
        }

        result += string;
        result += "\033[0m";

        return result;
    }

    inline void log(const std::string &str) {
        const std::string block = ansiColors("  sfml  ", {AnsiColor::Black, AnsiColor::BgCyan});

        std::cout << block << "\t" << str << std::endl;
    }

    inline void log(const std::string &str, AnsiColor bg) {
        const std::string block = ansiColors("  sfml  ", {AnsiColor::Black, bg});

        std::cout << block << "\t" << str << std::endl;
    }

    inline bool fileExists(const std::string &path) {
        return std::ifstream(path).good();
    }

    inline std::vector<std::string> loadFile(const std::string &path) {
        if (std::ifstream file(path); file.is_open()) {
            std::vector<std::string> result;
            std::string line;

            while (std::getline(file, line)) {
                result.push_back(line);
            }

            return result;
        }

        log("Failed to open file: " + path + "!", AnsiColor::BrightRed);
        return {};
    };
};
