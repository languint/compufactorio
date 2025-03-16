#include "util.h"
#include "lang/ast.h"
#include "lang/parser.h"

void printHeader() {
    util::ansiColors("Compufactorio!", {AnsiColor::BrightGreen});
    util::ansiColors("Compiled: " + static_cast<std::string>(__DATE__) + " " + __TIME__, {AnsiColor::BrightBlue});
    std::cout << "C++ Version: " << __cplusplus << "\n";
    std::cout << "---" << "\n";
    std::cout << "\n";
}

int main() {
    printHeader();
    const std::string code = R"(
        memset<int> counter = 0;

        func increment(int tick) {
            counter += 1;
        }

        events.register(Events::OnTick, increment);
    )";

    const std::vector<std::string> tokens = tokenize(code);

    Parser parser(tokens);

    if (const std::unique_ptr<ASTNode> ast = parser.parse()) {
        ast->print();
    }

    return 0;
}