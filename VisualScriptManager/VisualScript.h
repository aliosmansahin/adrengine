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
#include "ToString.h"
#include "SetPositionVector.h"
#include "MakeVector3.h"
#include "ConstFloat.h"
#include "AddImpulse.h"
#include "CastToObject.h"

#include "Logger.h"

#include "interfaces/IScene/IScene.h"

#include "interfaces/IVisualScript/IVisualScript.h"

class VisualScript : public IVisualScript
{
public:
	//main functions
	VISUALSCRIPTMANAGER_API bool		   CreateScript(std::string scriptId, std::string belongsEntity, std::string belongsScene) override;
	VISUALSCRIPTMANAGER_API void		   DrawScript();
	VISUALSCRIPTMANAGER_API void		   ReleaseScript() override;
	VISUALSCRIPTMANAGER_API void		   ExecuteBeginScript() override;
	//json
	VISUALSCRIPTMANAGER_API nlohmann::json ToJson() override;
	VISUALSCRIPTMANAGER_API void		   FromJson(nlohmann::json json, std::unordered_map<std::string, std::shared_ptr<INode>>& types, std::shared_ptr<IScene> scene) override;

	//Find
	VISUALSCRIPTMANAGER_API std::shared_ptr<IPin> FindPinById(int id) override;

	//Getters
	VISUALSCRIPTMANAGER_API std::string	   GetScriptId() override;
	VISUALSCRIPTMANAGER_API std::string	   GetBelongsEntity() override;
	VISUALSCRIPTMANAGER_API int GetNextId() override;
	VISUALSCRIPTMANAGER_API std::unordered_map<int, std::shared_ptr<NodeVisual>>& GetNodes() override;
	VISUALSCRIPTMANAGER_API std::unordered_map<int, std::pair<int, int>>& GetLinks() override;

	//Setters
	VISUALSCRIPTMANAGER_API void		   SetNextId(int nextId) override;

private:
	//script variables
	std::string scriptId = "";
	std::string belongsEntity = "";
	std::string belongsScene = "";

	//nodes
	std::unordered_map<int, std::shared_ptr<NodeVisual>> nodes;
	std::unordered_map<int, std::pair<int, int>> links;

	//The id will start from 1000 and will increment by the node
	int nextId = 1000;
};

