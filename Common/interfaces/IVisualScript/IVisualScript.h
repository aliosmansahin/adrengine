#pragma once

#include <string>
#include "../../nlohmann_json/json.hpp"

#include "INode/INode.h"

class Node; //TODO: Remove it when interface of node was implemented
class IScene;

class IVisualScript
{
public:
	virtual ~IVisualScript() = default;

	virtual bool CreateScript(std::string scriptId, std::string belongsEntity, std::string belongsScene) = 0;
	virtual void ReleaseScript() = 0;
	virtual void ExecuteBeginScript() = 0;

	virtual std::shared_ptr<IPin> FindPinById(int id) = 0;

	virtual std::string GetScriptId() = 0;
	virtual std::string GetBelongsEntity() = 0;
	virtual int GetNextId() = 0;
	virtual std::unordered_map<int, std::shared_ptr<NodeVisual>>& GetNodes() = 0;
	virtual std::unordered_map<int, std::pair<int, int>>& GetLinks() = 0;

	virtual void SetNextId(int nextId) = 0;
	
	virtual void FromJson(nlohmann::json json, std::unordered_map<std::string, std::shared_ptr<INode>>& types, std::shared_ptr<IScene> scene) = 0;
	virtual nlohmann::json ToJson() = 0;
};

