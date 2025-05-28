#pragma once

#include "../nlohmann_json/json.hpp"

class Entity;
class VisualScript;

namespace Utils {
    //All types of shaders
    enum ShaderType {
        SHADER_2D,
        SHADER_3D,
        DEPTH,
        DEPTH_CUBE
    };

    //This struct is for storing if the script belongs to an entity or a scene
    struct ScriptBelongsTo {
        Entity* entity = nullptr;
        nlohmann::json sceneJson;
    };


	//enum for scene types
	enum SceneType {
		SCENE_2D,
		SCENE_3D
	};

	//Tab objects
	enum TabType {
		SceneEditor,
		VisualScriptEditor
	};

	struct Tab {
		std::string id;
		TabType tabType;
	};

    /*
    PURPOSE: Creates a project json content
    TODO: This function will be moved to another place like "common" project (I haven't decided it yet)
    */
    inline nlohmann::json CreateProjectJson(
        std::string openedTabId,
        std::map<std::string, std::string>& scenes,
        std::unordered_map<std::string, std::shared_ptr<VisualScript>>& scripts,
        std::unordered_map<std::string, std::shared_ptr<Utils::Tab>>& tabs) {

        nlohmann::json projectJson;

        //Current tab
        projectJson["current-tab"] = openedTabId;

        //All scenes
        for (auto& sceneIter : scenes) {
            auto& scene = sceneIter.first;
            projectJson["scenes"].push_back(scene);
        }

        //All scripts
        for (auto& scriptIter : scripts) {
            auto& script = scriptIter.first;
            projectJson["scripts"].push_back(script);
        }

        //Opened tabs
        for (auto& tabIter : tabs) {
            auto& tabId = tabIter.first;
            auto& tab = tabIter.second;
            nlohmann::json a;
            a["id"] = tabId;
            if (tab->tabType == Utils::SceneEditor)
                a["type"] = "SceneEditor";
            if (tab->tabType == Utils::VisualScriptEditor)
                a["type"] = "VisualScriptEditor";
            projectJson["opened-tabs"].push_back(a);
        }

        return projectJson;
    }

}