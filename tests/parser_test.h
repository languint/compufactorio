#pragma once
#include "../tests.h"
#include "../lang/ast.h"

namespace parser_test {
    inline void run() {
        std::string code;

        std::vector<std::string> tokens = tokenize(code);
        auto parser = Parser(tokens);

        std::unique_ptr<ASTNode> ast = parser.parse();
        const auto *node = dynamic_cast<BlockNode *>(ast.get());

        tests::_assert(ast.get(), __LINE__);
        tests::_assert(node->statements.empty(), __LINE__);

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
        node = dynamic_cast<BlockNode *>(ast.get());

        tests::_assert(ast.get(), __LINE__);
        tests::_assert(node->statements.size() == 3, __LINE__);
    }

    inline tests::TestRegistrar registrar({__FILE_NAME__, run});
}
