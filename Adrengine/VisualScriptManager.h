#pragma once

#include "InterfaceManager.h"
#include "VisualScript.h"

class Scene;

struct ScriptBelongsTo {
	Entity* entity = nullptr;
	Scene* scene = nullptr;
};

class VisualScriptManager
{
public:
	bool InitManager();
	void ReleaseManager();
	std::shared_ptr<VisualScript> OpenScript(std::string scriptId);
	bool LoadScript(std::string scriptId);
	bool CloseScript(std::string scriptId);
	bool CreateScript(ScriptBelongsTo sbt);
	bool DeleteScript(std::string scriptId, ScriptBelongsTo sbt);
	std::string CompileScript();
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

