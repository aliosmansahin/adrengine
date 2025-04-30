#include "AssetSaver.h"

void AssetSaver::SaveEntityToFile(Entity* entity, const std::string& filepath) {
    std::ofstream file(filepath);
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for saving entity asset.");
        return;
    }

    nlohmann::json jsonData = entity->ToJson();

    std::cout << jsonData.dump(4) << std::endl;
    file << jsonData.dump(4);
}

std::shared_ptr<Entity> AssetSaver::LoadEntityFromFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for loading entity asset.");
        return nullptr;
    }

    nlohmann::json jsonData;
    file >> jsonData;
    
    std::shared_ptr<Entity> entity;
    if (!jsonData.is_null()) {
        std::string type = jsonData.value("type", "");
        if (type.empty())
            return nullptr;
        auto& types = Engine::GetInstance().entityTypes;
        auto typeIter = types.find(type);
        if (typeIter == types.end())
            return nullptr;
        
        entity = typeIter->second.first->clone();
        std::shared_ptr<EntityParams> params = typeIter->second.second->clone();
        params->FromJson(jsonData);
        entity->CreateEntity(params);
    }

    return entity;
}

void AssetSaver::SaveSceneToFile(Scene* scene, const std::string& filepath)
{
    std::ofstream file(filepath);
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for saving scene asset.");
        return;
    }

    if (scene->GetEntityManager()) {
        for (auto& entityIter : scene->GetEntityManager()->GetEntities()) {
            Entity* entity = entityIter.second.get();
            if (!entity) continue;

            auto params = entity->GetEntityParams();
            if (!params || params->id.empty()) continue;

            std::string projectDir = Engine::GetInstance().projectPath + Engine::GetInstance().projectName + "/";
            std::string entitiesDir = projectDir + "entities/";
            std::filesystem::create_directory(entitiesDir);

            std::cout << params->id.size() << std::endl;
            std::string entityDir = entitiesDir + params->id + "/";
            std::filesystem::create_directory(entityDir);

            std::string entityFile = entityDir + params->id + ".adrengineentity";
            SaveEntityToFile(entity, entityFile);
        }
    }

    nlohmann::json jsonData = scene->ToJson();
    file << jsonData.dump(4);
}

Scene* AssetSaver::LoadSceneFromFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for loading scene asset.");
        return nullptr;
    }

    nlohmann::json jsonData;
    file >> jsonData;

    if (jsonData.is_null())
        return nullptr;

    Scene* scene = new Scene();
    scene->FromJson(jsonData);
    //scene->CreateScene(jsonData["id"]);

    return scene;
}

void AssetSaver::SaveProjectToFile(const std::string& filepath)
{
    nlohmann::json j;
    if (InterfaceManager::GetInstance().openedTab) {
        j["current-tab"] = InterfaceManager::GetInstance().openedTab->id;
        //std::cout << InterfaceManager::GetInstance().openedTab->id << std::endl;
    }
    for (auto& sceneIter : SceneManager::GetInstance().scenes) {
        auto& scene = sceneIter.first;
        j["scenes"].push_back(scene);
    }
    for (auto& scriptIter : VisualScriptManager::GetInstance().scripts) {
        auto& script = scriptIter.first;
        j["scripts"].push_back(script);
    }
    for (auto& tabIter : InterfaceManager::GetInstance().tabs) {
        auto& tabId = tabIter.first;
        auto& tab = tabIter.second;
        std::string typeStr;
        if (tab->tabType == SceneEditor)
            typeStr = "SceneEditor";
        else if (tab->tabType == VisualScriptEditor)
            typeStr = "VisualScriptEditor";
        nlohmann::json a;
        a["id"] = tabId;
        a["type"] = typeStr;
        j["opened-tabs"].push_back(a);
    }
    std::ofstream file(filepath.c_str());
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for saving project asset.");
        return;
    }

    file << j.dump(4);
}

void AssetSaver::SaveScriptToFile(VisualScript* script, const std::string& filepath)
{
    std::ofstream file(filepath);
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for saving script asset.");
        return;
    }

    nlohmann::json jsonData = script->ToJson();
    file << jsonData.dump(4);
}

VisualScript* AssetSaver::LoadScriptFromFile(const std::string& filepath)
{
    std::ifstream file(filepath);
    if (!file.is_open()) {
        Logger::Log("E", "Unable to open file for loading script asset.");
        return nullptr;
    }

    nlohmann::json jsonData;
    file >> jsonData;

    VisualScript* script = new VisualScript();
    script->FromJson(jsonData);
    //scene->CreateScene(jsonData["id"]);

    return script;
}
