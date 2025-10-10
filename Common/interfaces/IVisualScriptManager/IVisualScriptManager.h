#pragma once

#include "../IVisualScript/IVisualScript.h"

#include "../../utils/Utils.h"

#include <memory>
#include <unordered_map>

class IScene;
class VisualScriptManager;

struct ImGuiContext;
struct ImNodesContext;

class IVisualScriptManager
{
public:
    //Handle manager
    virtual bool InitManager(ImGuiContext* imguiContext, ImNodesContext* nodesContext) = 0;
    virtual void ReleaseManager() = 0;

    //Handle scripts
	virtual std::shared_ptr<IVisualScript> LoadScript(std::string scriptId, std::shared_ptr<IScene> scene) = 0;
    virtual std::shared_ptr<IVisualScript> OpenScript(std::shared_ptr<IVisualScript> source) = 0;
    virtual bool CloseScript(std::string scriptId) = 0;
    virtual std::shared_ptr<IVisualScript> CreateScript(Utils::ScriptBelongsTo sbt) = 0;
    virtual bool DeleteScript(std::shared_ptr<IVisualScript> script) = 0;
    virtual bool SaveScript(std::shared_ptr<IVisualScript> script) = 0;

    //Getters
    virtual std::unordered_map<std::string, std::shared_ptr<IVisualScript>>& GetOpenedScripts() = 0;
    virtual std::shared_ptr<IVisualScript> GetCurrentScript() = 0;
    virtual std::unordered_map<std::string, std::shared_ptr<INode>>& GetTypes() = 0;

    //Setters
    virtual void SetCurrentScript(std::shared_ptr<IVisualScript> script) = 0;
};

