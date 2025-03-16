#pragma once
#include "../tests.h"

namespace ast_test {
    inline void run() {
        std::string code;

        std::vector<std::string> tokens = tokenize(code);
        tests::_assert(tokens.empty(), __LINE__);

        code = R"(
            memset<int> counter = 0;

            func increment(int tick) {
                counter += 1;
            }

            events.register(Events::OnTick, increment);
        )";
        tokens = tokenize(code);
        tests::_assert(tokens.size() == 28, __LINE__);
    }

    inline tests::TestRegistrar registrar({__FILE_NAME__, run});
}
