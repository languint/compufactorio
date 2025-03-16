#pragma once
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
    inline std::vector<std::string> split(const std::string &s, const std::string &delimiter) {
        size_t pos_start = 0, pos_end;
        const size_t delim_len = delimiter.length();
        std::vector<std::string> res;

        while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
            std::string token = s.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back(token);
        }

        res.push_back(s.substr(pos_start));
        return res;
    }

    inline void ansiColors(const std::string &string, const std::vector<AnsiColor>& codes) {
        std::string result;

        for (const auto &code: codes) {
            result += "\033[" + std::to_string(code) + "m";
        }

        result += string;
        result += "\033[0m";

        std::cout << result << std::endl;
    };
};