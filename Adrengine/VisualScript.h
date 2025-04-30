#pragma once

#include <string>

#include "nlohmann_json/json.hpp"

class VisualScript
{
public:
	bool CreateScript(std::string scriptId, std::string belongsTo, std::string belongsScene);
	void DrawScript();
	void ReleaseScript();
	void RunCompiled();
	nlohmann::json ToJson();
	void FromJson(nlohmann::json json);
public:
	std::string scriptId = "";
	std::string belongsTo = "";
	std::string belongsScene = "";

	std::vector<std::function<void()>> compiled;
};

