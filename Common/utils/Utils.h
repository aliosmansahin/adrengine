#pragma once

#include "../nlohmann_json/json.hpp"

namespace Utils {
    //All types of shaders
    enum ShaderType {
        SHADER_2D,
        SHADER_3D,
        DEPTH,
        DEPTH_CUBE,
        SHADER_INSPECT_TILE
    };

    //This struct is for storing if the script belongs to an entity or a scene
    struct ScriptBelongsTo {
        nlohmann::json entityJson;
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
        std::map<std::string, std::string>& scenes,
        std::string openedSceneId) {

        nlohmann::json projectJson;

        //Opened scene
        projectJson["opened-scene"] = openedSceneId;

        //All scenes
        for (auto& sceneIter : scenes) {
            auto& scene = sceneIter.first;
            projectJson["scenes"].push_back(scene);
        }
        
        return projectJson;
    }

}