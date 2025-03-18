#pragma once
#include <string>
#include <vector>

struct MapPosition {
    double x;
    double y;

    bool operator==(const MapPosition &position) const {
        return x == position.x && y == position.y;
    };

    MapPosition operator+(const MapPosition &position) const {
        return {x + position.x, y + position.y};
    }
};

struct BlueprintEntity {
    int entity_number;
    std::string name;
    MapPosition position;
};

/**
 * 0 - sourceEntityId
 * 1 - sourceWireType
 * 2 - targetEntityId
 * 3 - targetWireType
 */
using BlueprintWire = std::vector<int>;
