#include "tests.h"
#include "util.h"
#include "lang/ast.h"
#include "lang/parser.h"
#include "blueprint/blueprint.h"
#include "lang/type_checker.h"
#include "all_tests.h"

void printHeader() {
    util::ansiColors("Compufactorio!", {AnsiColor::BrightGreen});
    std::cout << "---" << "\n";
    std::cout << "\n";
}

int main() {
    printHeader();

#ifndef NDEBUG
    tests::runTests();
#endif

    const std::string code = R"(
        const memset<int> a = 3;
        input<int, WireColor::Green> b;
        output<int, WireColor::Red>(a * b);
    )";

    const std::vector<std::string> tokens = tokenize(code);

    Parser parser(tokens);

    if (const std::unique_ptr<ASTNode> ast = parser.parse()) {
        ast->print();
        TypeChecker checker;
        checker.check(ast.get(), false);
    }

    std::filesystem::path project_root = std::filesystem::current_path().parent_path();
    std::filesystem::path filepath = project_root / "blueprint/objects/clock-tile.json";
    std::ifstream file(filepath);
    std::cout << blueprint::parseJSON(filepath) << std::endl;

    return 0;
}
