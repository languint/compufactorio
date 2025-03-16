#pragma once

#include <chrono>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include <exception>
#include "util.h"

namespace tests {
    void _assert(auto value, const int line) {
        if (value) return;
        throw std::runtime_error("Assertion on line " + std::to_string(line) + " failed!");
    }

    using TestFunction = void(*)();
    using Test = std::pair<std::string, TestFunction>;

    inline std::vector<Test> &testRegistry() {
        static std::vector<Test> registry;
        return registry;
    }

    struct TestRegistrar {
        explicit TestRegistrar(const Test &testFunc) {
            testRegistry().push_back(testFunc);
        }
    };

    inline void printResult(const std::string &name, bool success, const std::string &message) {
        const std::string printMessage = success ? "succeeded." : "failed!";
        const AnsiColor color = success ? AnsiColor::Green : AnsiColor::BrightRed;
        util::ansiColors("[TESTS]: " + name + " " + printMessage, {color});
        if (!success && !message.empty()) {
            util::ansiColors("\t" + message, {color});
        }
    }

    inline void runTests() {
        const auto start = std::chrono::system_clock::now();
        util::ansiColors("[TESTS]: Running " + std::to_string(testRegistry().size()) + " test(s).\n",
                         {AnsiColor::BrightBlue});

        int totalSuccesses = 0;
        int totalFailures = 0;

        for (const auto &[name, testFunc]: testRegistry()) {
            try {
                testFunc();
                printResult(name, true, "");
                totalSuccesses += 1;
            } catch (const std::exception &e) {
                printResult(name, false, e.what());
                totalFailures += 1;
            }
        }

        AnsiColor finalColor = totalFailures > 0 ? AnsiColor::Red : AnsiColor::BrightWhite;

        util::ansiColors(
            "[TESTS]: Tests succeeded: " + std::to_string(totalSuccesses) + ", Tests failed: " +
            std::to_string(totalFailures), {finalColor});
        const auto end = std::chrono::system_clock::now();
        const auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "\tTests ran in " << duration.count() << "µs" << std::endl;
    }
} // namespace tests
