#include "util.h"

void printHeader() {
    util::ansiColors("Compufactorio!", {AnsiColor::BrightGreen});
    util::ansiColors("Compiled: " + static_cast<std::string>(__DATE__) + " " + __TIME__, {AnsiColor::BrightBlue});
    std::cout << "C++ Version: " << __cplusplus << "\n";
    std::cout << "---" << "\n";
    std::cout << "\n";
}

int main() {
    printHeader();
    return 0;
}

