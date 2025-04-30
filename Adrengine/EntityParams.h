#pragma once

#include <string>
#include <vector>

#include "nlohmann_json/json.hpp"

class Entity;
class VisualScript;

class EntityParams {
public:
	virtual ~EntityParams() = default;
	virtual std::string GetType() {
		return "Entity";
	}
	virtual std::shared_ptr<EntityParams> clone() const {
		return std::make_shared<EntityParams>(*this);
	}
	float x = 0.0f, y = 0.0f, z = 0.0f;
	float sx = 1.0f, sy = 1.0f, sz = 0.0f;
	float rx = 0.0f, ry = 0.0f, rz = 0.0f;
	std::string id = "";
	std::string name = "";
	std::shared_ptr<Entity> parent;
	std::string parentId = "";

	std::vector<std::shared_ptr<Entity>> children;

	std::string scriptId = "";
	VisualScript* script;

	virtual nlohmann::json ToJson();

	virtual void FromJson(const nlohmann::json& j);
};