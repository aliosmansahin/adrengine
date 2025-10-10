#pragma once

#ifdef VISUALSCRIPTMANAGER_EXPORTS
#define VISUALSCRIPTMANAGER_API __declspec(dllexport)
#else
#define VISUALSCRIPTMANAGER_API __declspec(dllimport)
#endif

#include "VisualScript.h"
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#include "imgui/imgui_internal.h"

#include "imnodes/imnodes.h"

#include "AssetSaver.h"
#include "Logger.h"

#include "utils/Utils.h"

#include "interfaces/IVisualScriptManager/IVisualScriptManager.h"
#include "interfaces/IEngine/IEngine.h"
#include "interfaces/IProject/IProject.h"

#include "ServiceLocator.h"

class VisualScriptManager : public IVisualScriptManager
{
public:
	//main functions
	VISUALSCRIPTMANAGER_API bool InitManager(ImGuiContext* imguiContext, ImNodesContext* nodesContext) override;
	VISUALSCRIPTMANAGER_API void ReleaseManager() override;

	//script functions
	VISUALSCRIPTMANAGER_API std::shared_ptr<IVisualScript> OpenScript(std::shared_ptr<IVisualScript> source) override;
	VISUALSCRIPTMANAGER_API std::shared_ptr<IVisualScript> LoadScript(std::string scriptId, std::shared_ptr<IScene> scene) override;
	VISUALSCRIPTMANAGER_API bool						   CloseScript(std::string scriptId) override;
	VISUALSCRIPTMANAGER_API std::shared_ptr<IVisualScript> CreateScript(Utils::ScriptBelongsTo sbt) override;
	VISUALSCRIPTMANAGER_API bool						   DeleteScript(std::shared_ptr<IVisualScript> script) override;
	VISUALSCRIPTMANAGER_API bool					       SaveScript(std::shared_ptr<IVisualScript> script) override;

	//Getters
	VISUALSCRIPTMANAGER_API std::unordered_map<std::string, std::shared_ptr<IVisualScript>>& GetOpenedScripts() override;
	VISUALSCRIPTMANAGER_API std::shared_ptr<IVisualScript> GetCurrentScript() override;
	VISUALSCRIPTMANAGER_API std::unordered_map<std::string, std::shared_ptr<INode>>& GetTypes() override;

	//Setters
	VISUALSCRIPTMANAGER_API void SetCurrentScript(std::shared_ptr<IVisualScript> script) override;

public:
	//Singleton
	static VisualScriptManager& GetInstance() {
		static VisualScriptManager instance;
		return instance;
	}

private:
	//helper
	void InitializeNodeTypes();

public:
	//singleton
	VisualScriptManager() = default;
	~VisualScriptManager() = default;
	VisualScriptManager(const VisualScriptManager&) = delete;
	VisualScriptManager& operator=(const VisualScriptManager&) = delete;

private:
	//scripts
	std::unordered_map<std::string, std::shared_ptr<IVisualScript>> openedScripts;
	std::shared_ptr<IVisualScript> currentScript = nullptr;

	//node types
	std::unordered_map<std::string, std::shared_ptr<INode>> types;
};

