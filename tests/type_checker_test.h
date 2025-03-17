#pragma once
#include "../tests.h"
#include "../lang/ast.h"
#include "../lang/parser.h"
#include "../lang/type_checker.h"

namespace type_checker_test {
    inline void run() {
        std::string code;

        std::vector<std::string> tokens = tokenize(code);
        auto parser = Parser(tokens);

        std::unique_ptr<ASTNode> ast = parser.parse();
        auto typeChecker = TypeChecker();

        tests::_assert(ast.get(), __LINE__);
        tests::_assert(!typeChecker.check(ast.get(), true), __LINE__);

        code = R"(
            memset<int> counter = 0;

            func increment(int tick) {
                counter += 1;
            }

            events.register(Events::OnTick, increment);
        )";

        tokens = tokenize(code);
        parser = Parser(tokens);
        ast = parser.parse();

        tests::_assert(ast.get(), __LINE__);
        tests::_assert(!typeChecker.check(ast.get(), true), __LINE__);

        code = R"(
            const memset<int> var = 0;

            func increment(int tick) {
                var = 10;
            }
        )";

        tokens = tokenize(code);
        parser = Parser(tokens);
        ast = parser.parse();

        // This should fail because you cannot assign to a constant.
        tests::_assert(ast.get(), __LINE__);
        tests::_assert(typeChecker.check(ast.get(), true), __LINE__);
    }

    inline tests::TestRegistrar registrar({__FILE_NAME__, run});
}
