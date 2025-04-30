#pragma once

#include <unordered_map>
#include <string>
#include <fstream>

#include "nlohmann_json/json.hpp"

class AssetIndexer {
private:
    std::unordered_map<std::string, std::string> index;

public:
    void AddEntry(const std::string& id, const std::string& path) {
        index[id] = path;
    }

    void SaveToFile(const std::string& filepath) const {
        std::ofstream file(filepath);
        if (!file.is_open()) return;

        nlohmann::json j;
        for (const auto& [id, path] : index) {
            j[id] = path;
        }

        file << j.dump(4);
    }
};