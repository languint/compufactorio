#pragma once
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

struct MapPosition {
    double x;
    double y;
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

class Blueprint {
public:
    std::vector<BlueprintEntity> entities;
    std::vector<BlueprintWire> wires;
    std::string item;
    std::string label;
    int version = 0;

    Blueprint() = default;

    BlueprintEntity* getEntity(const int entity_number) {
        for (auto &entity: entities) {
            if (entity.entity_number == entity_number) {
                return &entity;
            }
        }
        std::cerr << "Entity: " << entity_number << " not found" << std::endl;
        return nullptr;
    };

    void setEntity(const int entity_number, const BlueprintEntity& entity) {
        entities[entity_number] = entity;
    }
};
