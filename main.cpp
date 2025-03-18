#include "tests.h"
#include "util.h"
#include "lang/ast.h"
#include "lang/parser.h"
#include "blueprint/blueprint.h"
#include "lang/type_checker.h"
#include "all_tests.h"
#include "blueprint/tiler.h"

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
    std::filesystem::path filepath = ROOT / "blueprint/objects/tile.json";
    std::ifstream file(filepath);
    const json j = blueprint::io::loadFile(filepath);

    Blueprint b = blueprint::createBlueprintFromJSON(j.at("blueprint"));
    tiler::addTile(b, Tile{
                       {1, 0},
                       RAM
                   });

    std::cout << blueprint::io::encode(blueprint::createJSONFromBlueprint(b)) << std::endl;

    return 0;
}
