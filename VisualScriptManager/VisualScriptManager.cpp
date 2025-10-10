#include "pch.h"
#include "VisualScriptManager.h"

/*
PURPOSE: Initializes the manager
*/
bool VisualScriptManager::InitManager(ImGuiContext* imguiContext, ImNodesContext* nodesContext)
{
    //Set imgui and imnodes contexts
    ImGui::SetCurrentContext(imguiContext);
    ImNodes::SetCurrentContext(nodesContext);

    //Create all node types
    InitializeNodeTypes();

	return true;
}

/*
PURPOSE: Releases all visual script stuff
*/
void VisualScriptManager::ReleaseManager()
{
    //currentScript is nullptr anymore
    currentScript = nullptr;

    //Clear script "unordered_map"s
    openedScripts.clear();
    types.clear();

    //logger
    Logger::Log("P", "Cleared visual script manager");
}

/*
PURPOSE: Opens the script to edit
	Returns nullptr if the script is already opened
*/
std::shared_ptr<IVisualScript> VisualScriptManager::OpenScript(std::shared_ptr<IVisualScript> source)
{
    //If the script is already opened, interrupt the function
    auto openedScriptIter = openedScripts.find(source->GetScriptId());
    if(openedScriptIter != openedScripts.end())
        return nullptr;

	//Set the current script
    currentScript = source;

    //Open the script
    openedScripts.insert(std::pair<std::string, std::shared_ptr<IVisualScript>>(source->GetScriptId(), source));

    //Return the script
    return source;
}

/*
PURPOSE: Loads the script to run
*/
std::shared_ptr<IVisualScript> VisualScriptManager::LoadScript(std::string scriptId, std::shared_ptr<IScene> scene)
{
    //Load the script
    nlohmann::json scriptJson = AssetSaver::LoadScriptFromFile(ServiceLocator::Get<IProject>()->GetProjectDir(), scriptId);

    if (scriptJson.is_null())
        return nullptr;

    //Create a script object and insert it to scripts
    std::shared_ptr<IVisualScript> script = std::make_shared<VisualScript>();
    script->FromJson(scriptJson, types, scene);

    return std::shared_ptr<IVisualScript>(script);
}

/*
PURPOSE: Saves and closes the script
*/
bool VisualScriptManager::CloseScript(std::string scriptId)
{
    //Check if script is opened
    auto openedScript = openedScripts.find(scriptId);
    if (openedScript == openedScripts.end()) {
        //This script is not opened
        std::string str = "There is not any script which has given id \"";
        str += scriptId;
        str += "\"";
        Logger::Log("E", str.c_str());
        return false;
    }
    else {
        //This script is opened
        auto& script = openedScript->second;

        //Save the script
        AssetSaver::SaveScriptToFile(script->ToJson(), ServiceLocator::Get<IProject>()->GetProjectDir(), script->GetScriptId());

        //Remove the tab and script from tabs and openedScripts
        openedScripts.erase(openedScript);
    }
	return true;
}

/*
PURPOSE: Creates a new script
*/
std::shared_ptr<IVisualScript> VisualScriptManager::CreateScript(Utils::ScriptBelongsTo sbt)
{
    //Set the id
    int index = 0;
    std::string scriptId;
    while (true) {
        scriptId = "script" + std::to_string(index);

        std::string scriptsDir = ServiceLocator::Get<IProject>()->GetProjectDir() + "scripts/";
        std::string scriptDir = scriptsDir + scriptId + "/";

        if (!std::filesystem::exists(scriptDir))
            break;

        ++index;
    }
    
    std::string entityId = sbt.entityJson["id"];
    std::string sceneId = sbt.sceneJson["id"];

    std::shared_ptr<IVisualScript> source = std::make_shared<VisualScript>();

    source->CreateScript(scriptId, entityId, sceneId);
    
    //Insert the script to "unordered_map"s
    openedScripts.insert(std::pair<std::string, std::shared_ptr<IVisualScript>>(scriptId, source));

    //Save the script
    AssetSaver::SaveScriptToFile(source->ToJson(), ServiceLocator::Get<IProject>()->GetProjectDir(), scriptId);

    //Save the scene
    AssetSaver::SaveSceneToFile(sbt.sceneJson, ServiceLocator::Get<IProject>()->GetProjectDir(), sceneId);

    //Set the current
    currentScript = source;

    return source;
}

/*
PURPOSE: Deletes the script
*/
bool VisualScriptManager::DeleteScript(std::shared_ptr<IVisualScript> script)
{
    //Release script
    script->ReleaseScript();

    //Delete directory of the script
    std::string scriptsDir = ServiceLocator::Get<IProject>()->GetProjectDir() + "scripts/";
    std::string scriptDir = scriptsDir + script->GetScriptId() + "/";

    std::filesystem::remove_all(scriptDir);

    //Delete opened script
    auto scriptIter = VisualScriptManager::GetInstance().GetOpenedScripts().find(script->GetScriptId());
    if (scriptIter != VisualScriptManager::GetInstance().GetOpenedScripts().end()) {
        VisualScriptManager::GetInstance().GetOpenedScripts().erase(script->GetScriptId());
    }

    std::string str = "Deleted script \"";
    str += script->GetScriptId();
    str += "\"";
    Logger::Log("P", str.c_str());
	return true;
}

/*
PURPOSE: Saves the script
*/
bool VisualScriptManager::SaveScript(std::shared_ptr<IVisualScript> script)
{
    //Saves the script
    AssetSaver::SaveScriptToFile(script->ToJson(), ServiceLocator::Get<IProject>()->GetProjectDir(), script->GetScriptId());

    return true;
}

/*
PURPOSE: Getter for openedScripts
*/
VISUALSCRIPTMANAGER_API std::unordered_map<std::string, std::shared_ptr<IVisualScript>>& VisualScriptManager::GetOpenedScripts()
{
    return openedScripts;
}

/*
PURPOSE: Returns current script as a smart pointer
*/
VISUALSCRIPTMANAGER_API std::shared_ptr<IVisualScript> VisualScriptManager::GetCurrentScript()
{
    return currentScript;
}

/*
PURPOSE: Returns all types of nodes as an unordered_map
*/
VISUALSCRIPTMANAGER_API std::unordered_map<std::string, std::shared_ptr<INode>>& VisualScriptManager::GetTypes()
{
    return types;
}

/*
PURPOSE: Sets the current script
*/
VISUALSCRIPTMANAGER_API void VisualScriptManager::SetCurrentScript(std::shared_ptr<IVisualScript> script)
{
    currentScript = script;
}

/*
PURPOSE: Initializes all node types
    They will be copied to the node that will be added
*/
void VisualScriptManager::InitializeNodeTypes()
{
    types = { {
        {"Begin", std::make_shared<Begin>()},
        {"Print", std::make_shared<Print>()},
        {"ConstString", std::make_shared<ConstString>()},
        {"GetThisEntity", std::make_shared<GetThisEntity>()},
        {"GetPositionVector", std::make_shared<GetPositionVector>()},
        {"SeparateVector3", std::make_shared<SeparateVector3>()},
        {"ToString", std::make_shared<ToString>()},
        {"SetPositionVector", std::make_shared<SetPositionVector>()},
        {"MakeVector3", std::make_shared<MakeVector3>()},
        {"ConstFloat", std::make_shared<ConstFloat>()},
        {"AddImpulse", std::make_shared<AddImpulse>()},
        {"CastToObject", std::make_shared<CastToObject>()},
    } };
}
