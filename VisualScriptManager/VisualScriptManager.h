#pragma once

#ifdef VISUALSCRIPTMANAGER_EXPORTS
#define VISUALSCRIPTMANAGER_API __declspec(dllexport)
#else
#define VISUALSCRIPTMANAGER_API __declspec(dllimport)
#endif

#include "VisualScript.h"
#include "Entity.h"
#include "AssetSaver.h"
#include "Logger.h"

class Scene;

//This struct is for storing if the script belongs to an entity or a scene
struct ScriptBelongsTo {
	Entity* entity = nullptr;
	nlohmann::json sceneJson;
};

enum TabType;

//THIS WILL BE MOVED TO ANOTHER PLACE LIKE "COMMON"
VISUALSCRIPTMANAGER_API nlohmann::json CreateProjectJson(
	std::string openedTabId,
	std::map<std::string, std::string>& scenes,
	std::unordered_map<std::string, std::shared_ptr<VisualScript>>& scripts,
	std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs);

class VisualScriptManager
{
public:
	//main functions
	VISUALSCRIPTMANAGER_API bool														   InitManager();
	VISUALSCRIPTMANAGER_API void														   ReleaseManager();

	//script functions
	VISUALSCRIPTMANAGER_API std::pair<std::shared_ptr<VisualScript>, std::shared_ptr<Tab>> OpenScript(std::string scriptId, std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs);
	VISUALSCRIPTMANAGER_API bool														   LoadScript(std::string scriptId, std::string& projectDir, std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs);
	VISUALSCRIPTMANAGER_API bool														   CloseScript(
		std::string scriptId,
		std::string& projectDir,
		std::unordered_map<std::string,
		std::shared_ptr<Tab>>&tabs,
		Tab*& openedTab,
		std::map<std::string, std::string>& scenes
	);
	VISUALSCRIPTMANAGER_API bool														   CreateScript(ScriptBelongsTo sbt, std::string& projectDir, std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs,
		Tab*& openedTab, std::string& selectedTabId, std::map<std::string, std::string>& scenes);
	VISUALSCRIPTMANAGER_API bool														   DeleteScript(std::string scriptId, ScriptBelongsTo sbt);
	VISUALSCRIPTMANAGER_API bool														   SaveScript(
		std::shared_ptr<VisualScript> script,
		std::string& projectDir,
		Tab*& openedTab,
		std::map<std::string, std::string>& scenes,
		std::unordered_map<std::string, std::shared_ptr<Tab>>& tabs
		);

	//compilation
	VISUALSCRIPTMANAGER_API void														   RunScriptsBegin();

	//getter for the instance
	VISUALSCRIPTMANAGER_API static VisualScriptManager&                                    GetInstance();
private:
	//singleton
	VisualScriptManager() = default;
	~VisualScriptManager() = default;
	VisualScriptManager(const VisualScriptManager&) = delete;
	VisualScriptManager& operator=(const VisualScriptManager&) = delete;
public:
	//scripts
	std::unordered_map<std::string, std::shared_ptr<VisualScript>> openedScripts;
	std::unordered_map<std::string, std::shared_ptr<VisualScript>> scripts;
	std::shared_ptr<VisualScript> currentScript = nullptr;

	//node types
	std::unordered_map<std::string, std::shared_ptr<Node>> types;
};

