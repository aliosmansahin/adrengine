#pragma once

#ifdef INTERFACEMANAGER_EXPORTS
#define INTERFACEMANAGER_API __declspec(dllexport)
#else
#define INTERFACEMANAGER_API __declspec(dllimport)
#endif

#include <memory>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"

#include "utils/Utils.h"

#include "interfaces/IEntity/IEntity.h"
#include "interfaces/IEntity/IObject/IObject.h"
#include "interfaces/IEntity/ISprite2D/ISprite2D.h"
#include "interfaces/IEntity/ILight/IDirectionalLight/IDirectionalLight.h"
#include "interfaces/IEntity/ILight/ISpotLight/ISpotLight.h"
#include "interfaces/IEntity/ILight/IPointLight/IPointLight.h"
#include "interfaces/IEntity/ISprite2D/ITileMap/ITileMap.h"
#include "interfaces/IEntity/ISprite2D/IFlipBook/IFlipBook.h"

#include "interfaces/ISceneManager/ISceneManager.h"
#include "interfaces/IVisualScriptManager/IVisualScriptManager.h"
#include "interfaces/IProject/IProject.h"

#include "AssetSaver.h"
#include "AssetDatabase.h"

#include "ServiceLocator.h"

#include "Localization.h"

class WindowEntityProperties
{
public:
	//main functions
	INTERFACEMANAGER_API void DrawWindow();
	INTERFACEMANAGER_API void SelectEntity(std::shared_ptr<IEntity> entity);
	INTERFACEMANAGER_API static WindowEntityProperties& GetInstance();
	INTERFACEMANAGER_API std::string				 GetWindowId() {
		return "window_entity_properties";
	}
	INTERFACEMANAGER_API std::string GetWindowTitle() {
		return Localization::GetString("interface_main_menu_item_entity_properties");
	}
	INTERFACEMANAGER_API std::string GetWindowTitleWithID() {
		return GetWindowTitle() + "###" + GetWindowId();
	}
private:
	//singleton
	WindowEntityProperties() = default;
	~WindowEntityProperties() = default;
	WindowEntityProperties(const WindowEntityProperties&) = delete;
	WindowEntityProperties& operator=(const WindowEntityProperties&) = delete;
public:
	//variables
	bool showWindow = true;
	std::shared_ptr<IEntity> currentEntity;
};