#pragma once

#ifdef VISUALSCRIPTMANAGER_EXPORTS
#define VISUALSCRIPTMANAGER_API __declspec(dllexport)
#else
#define VISUALSCRIPTMANAGER_API __declspec(dllimport)
#endif

#include <string>

#include "nlohmann_json/json.hpp"
#include "Begin.h"
#include "Print.h"
#include "ConstString.h"
#include "GetThisEntity.h"
#include "GetPositionVector.h"
#include "SeparateVector3.h"

#include "SceneManager.h"

class VisualScript
{
public:
	//main functions
	VISUALSCRIPTMANAGER_API bool		   CreateScript(std::string scriptId, std::string belongsTo, std::string belongsScene);
	VISUALSCRIPTMANAGER_API void		   DrawScript();
	VISUALSCRIPTMANAGER_API void		   ReleaseScript();
	VISUALSCRIPTMANAGER_API void		   ExecuteBeginScript();
	//json
	VISUALSCRIPTMANAGER_API nlohmann::json ToJson();
	VISUALSCRIPTMANAGER_API void		   FromJson(nlohmann::json json, std::unordered_map<std::string, std::shared_ptr<Node>>& types);

	//Find
	VISUALSCRIPTMANAGER_API Pin*		   FindPinById(int id);
public:
	//script variables
	std::string scriptId = "";
	std::string belongsTo = "";
	std::string belongsScene = "";

	//nodes
	std::unordered_map<int, NodeVisual> nodes;
	std::unordered_map<int, std::pair<int, int>> links;

	//The id will start from 1000 and will increment by the node
	int nextId = 1000;
};

