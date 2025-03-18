#pragma once
#include "../tests.h"
#include "../blueprint/blueprint.h"

namespace blueprint_test {
    inline void run() {
        std::filesystem::path filepath = ROOT / "blueprint/objects/tile.json";
        std::ifstream file(filepath);
        const json j = blueprint::io::loadFile(filepath);
        auto b = blueprint::createBlueprintFromJSON(j.at("blueprint"));

        auto encoded = blueprint::io::encode(blueprint::createJSONFromBlueprint(b));

        tests::_assert(b.entities.size() == 4, __LINE__);
        tests::_assert(
            encoded ==
            "0eNrFU8sKhCAU3fcV4TqGSoOhX4kYClwIZaE2MxH9+2hBD3LhYJarPOfmuec+Bs+XB5RVh1tGqACpP0zQBGMqiCCYSzRbUHWG3W2N7V+0q0vM5A9RcIyhRY0lBXhXclEI0lCgiWobTiYu1ehMEV9JhY8w0LP9zB7IcYeMwb+OYseOouflluCtTVKGT7eE7u2Siafllq8PASJwrXJcV3FDVkWJK8VuwTdmfE53kxD4EKZZ2OyQk2Y/NZBm5qMdkgen6EAXOrEZ5EIHutBJjKQTFzrQWic2mgNkPQdmTUfW/YmN6oas6wYvqhu8qG7wpLp5+6/RG39rIWRY",
            __LINE__);
    }

    inline tests::TestRegistrar registrar({__FILE_NAME__, run});
}
