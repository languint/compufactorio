#pragma once
#include <fstream>
#include <string>
#include "nlohmann/json.hpp"
#include "zlib.h"
#include "../base64.h"
#include "types.h"

using json = nlohmann::json;

inline std::filesystem::path ROOT = std::filesystem::current_path().parent_path();

class Blueprint {
public:
    std::vector<BlueprintEntity> entities;
    std::vector<BlueprintWire> wires;
    std::string item;
    std::string label;
    int version = 0;

    Blueprint() = default;

    BlueprintEntity *getEntity(const int entity_number) {
        for (auto &entity: entities) {
            if (entity.entity_number == entity_number) {
                return &entity;
            }
        }
        std::cerr << "Entity: " << entity_number << " not found" << std::endl;
        return nullptr;
    };

    BlueprintEntity *getEntity(const MapPosition &position) {
        for (auto &entity: entities) {
            if (entity.position == position) {
                return &entity;
            }
        }
        return nullptr;
    }

    void setEntity(const int entity_number, const BlueprintEntity &entity) {
        entities[entity_number] = entity;
    }

    void addEntity(const BlueprintEntity &entity) {
        entities.push_back(entity);
    }

    [[nodiscard]] int getNextEntityNumber() const {
        return entities.back().entity_number + 1;
    }

    void shift(const MapPosition offset) {
        for (auto &entity: entities) {
            entity.position = entity.position + offset;
        }
    }

    [[nodiscard]] Blueprint copy() const {
        const auto bp = new Blueprint();
        bp->entities = this->entities;
        bp->wires = this->wires;
        bp->item = this->item;
        bp->label = this->label;
        bp->version = this->version;

        return *bp;
    }
};


namespace blueprint::io {
    inline json loadFile(const std::string &path) {
        std::ifstream file(path);
        return json::parse(file);
    }

    inline std::string compressString(const std::string &input) {
        uLongf compressed_size = compressBound(input.size());
        std::vector<unsigned char> compressed_data(compressed_size);

        const int res = compress2(compressed_data.data(), &compressed_size,
                                  reinterpret_cast<const Bytef *>(input.data()),
                                  input.size(), Z_BEST_COMPRESSION);
        if (res != Z_OK) {
            throw std::runtime_error("Compression failed");
        }

        compressed_data.resize(compressed_size);
        return std::string{compressed_data.begin(), compressed_data.end()};
    }

    inline std::string encode(const json &j) {
        const std::string str = j.dump(4);
        const std::string compressed = compressString(str);
        return "0" + base64::to_base64(compressed);
    }
}

namespace blueprint {
    inline MapPosition parsePosition(const json &j) {
        return MapPosition{
            j.at("x").get<double>(),
            j.at("y").get<double>()
        };
    }

    inline BlueprintEntity parseEntity(const json &j) {
        BlueprintEntity entity;
        entity.entity_number = j.at("entity_number").get<int>();
        entity.name = j.at("name").get<std::string>();
        entity.position = parsePosition(j.at("position"));

        return entity;
    }

    inline BlueprintWire parseWire(const json &j) {
        BlueprintWire wire;


        for (const auto &v: j.get<std::vector<int> >()) {
            wire.push_back(v);
        }

        return wire;
    }

    inline Blueprint createBlueprintFromJSON(const json &j) {
        Blueprint bp;
        auto entities = j.at("entities").get<std::vector<json> >();
        std::vector<BlueprintEntity> parsedEntities;
        for (const auto &e: entities) {
            parsedEntities.push_back(parseEntity(e));
        }

        bp.entities = parsedEntities;
        auto wires = j.at("wires").get<std::vector<json> >();

        std::vector<BlueprintWire> parsedWires;
        for (const auto &w: wires) {
            parsedWires.push_back(parseWire(w));
        }
        bp.wires = parsedWires;

        return bp;
    }

    inline json createJSONFromBlueprint(const Blueprint &bp) {
        json blueprintJson;
        blueprintJson["entities"] = json::array();

        for (const auto &[entity_number, name, position]: bp.entities) {
            json entityJson;
            entityJson["entity_number"] = entity_number;
            entityJson["name"] = name;
            entityJson["position"] = {
                {"x", position.x},
                {"y", position.y}
            };
            blueprintJson["entities"].push_back(entityJson);
        }

        blueprintJson["wires"] = bp.wires;
        blueprintJson["item"] = "blueprint";
        blueprintJson["label"] = bp.label;
        blueprintJson["version"] = bp.version;

        return json{{"blueprint", blueprintJson}};
    }
}
