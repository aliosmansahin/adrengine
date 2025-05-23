#pragma once

#ifdef VISUALSCRIPTMANAGER_EXPORTS
#define VISUALSCRIPTMANAGER_API __declspec(dllexport)
#else
#define VISUALSCRIPTMANAGER_API __declspec(dllimport)
#endif

#include <string>

#include "nlohmann_json/json.hpp"
#include "Begin.h"
#include "Return.h"
#include "Print.h"

class VisualScript
{
public:
	//main functions
	VISUALSCRIPTMANAGER_API bool		   CreateScript(std::string scriptId, std::string belongsTo, std::string belongsScene);
	VISUALSCRIPTMANAGER_API void		   DrawScript();
	VISUALSCRIPTMANAGER_API void		   ReleaseScript();

	//compilation
	VISUALSCRIPTMANAGER_API void		   RunCompiled();
	VISUALSCRIPTMANAGER_API std::string    CompileScript();

	//json
	VISUALSCRIPTMANAGER_API nlohmann::json ToJson();
	VISUALSCRIPTMANAGER_API void		   FromJson(nlohmann::json json, std::unordered_map<std::string, std::shared_ptr<Node>>& types);
public:
	//script variables
	std::string scriptId = "";
	std::string belongsTo = "";
	std::string belongsScene = "";

	//compilation
	std::vector<std::function<void()>> compiled;

	//nodes
	std::unordered_map<int, std::shared_ptr<Node>> nodes;
	std::unordered_map<int, std::pair<int, int>> links;
};

