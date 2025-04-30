#pragma once

#include "InterfaceManager.h"
#include "VisualScript.h"

class Scene;
struct Tab;

struct ScriptBelongsTo {
	Entity* entity = nullptr;
	Scene* scene = nullptr;
};

class VisualScriptManager
{
public:
	bool InitManager();
	void ReleaseManager();
	std::pair<std::shared_ptr<VisualScript>, std::shared_ptr<Tab>> OpenScript(std::string scriptId);
	bool LoadScript(std::string scriptId);
	bool CloseScript(std::string scriptId);
	bool CreateScript(ScriptBelongsTo sbt);
	bool DeleteScript(std::string scriptId, ScriptBelongsTo sbt);
	bool SaveScript(std::shared_ptr<VisualScript> script);
	std::string CompileScript(VisualScript* script);
	void RunScriptsBegin();
	static VisualScriptManager& GetInstance();
private:
	VisualScriptManager() = default;
	~VisualScriptManager() = default;
	VisualScriptManager(const VisualScriptManager&) = delete;
	VisualScriptManager& operator=(const VisualScriptManager&) = delete;
public:
	//scripts
	std::unordered_map<std::string, std::shared_ptr<VisualScript>> openedScripts;
	std::unordered_map<std::string, std::shared_ptr<VisualScript>> scripts;
	std::shared_ptr<VisualScript> currentScript = nullptr;

	std::unordered_map<std::string, std::shared_ptr<Node>> types;
};

