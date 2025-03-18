#pragma once
#include "blueprint.h"

struct TilePosition {
    double x;
    double y;

    MapPosition operator+(const TilePosition &tile_position) const {
        return {x + tile_position.x, y + tile_position.y};
    }

    MapPosition operator*(const int i) const {
        return {x * i, y * i};
    };
};

enum TileType {
    RESERVED,
    RAM,
};

struct Tile {
    TilePosition position;
    TileType type;
};

constexpr int TILE_SIZE = 18;

namespace tiler {
    inline std::pair<int, int> addTile(Blueprint &bp, Tile tile) {
        std::filesystem::path tileBlueprintPath = ROOT / "blueprint/objects/tile.json";
        std::ifstream file(tileBlueprintPath);

        const json j = blueprint::io::loadFile(tileBlueprintPath).at("blueprint");

        int createdInstances = 0, notCreatedInstances = 0;

        for (Blueprint tileBlueprint = blueprint::createBlueprintFromJSON(j); const auto &entity: tileBlueprint.
             entities) {
            if (const MapPosition newPosition = entity.position + (tile.position * TILE_SIZE); bp.
                getEntity(newPosition)) {
                notCreatedInstances++;
            } else {
                createdInstances++;
                bp.addEntity({
                    bp.getNextEntityNumber(),
                    entity.name,
                    newPosition,
                });
            }
        };

        return {createdInstances, notCreatedInstances};
    }
}
