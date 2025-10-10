#pragma once

#ifdef INTERFACEMANAGER_EXPORTS
#define INTERFACEMANAGER_API __declspec(dllexport)
#else
#define INTERFACEMANAGER_API __declspec(dllimport)
#endif

#include "Graphics.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Localization.h"
#include "InputManager.h"
#include "Logger.h"

#include "interfaces/IEntity/ISprite2D/ITileMap/ITileMap.h"
#include "interfaces/ISceneManager/ISceneManager.h"

#include "ServiceLocator.h"

class WindowTileMapViewer
{
public:
	INTERFACEMANAGER_API void DrawWindow();

	//singleton
	INTERFACEMANAGER_API static WindowTileMapViewer& GetInstance();
private:
	WindowTileMapViewer() = default;
	~WindowTileMapViewer() = default;
	WindowTileMapViewer(const WindowTileMapViewer&) = delete;
	WindowTileMapViewer operator=(const WindowTileMapViewer&) = delete;
public:
	bool showWindow = false;
	std::shared_ptr<ITileMap> edittingTileMap = nullptr;
	int window_width = 0;
	int window_height = 0;

	//framebuffer
	std::unique_ptr<FramebufferProvider> framebuffer;

	//camera
	float cameraX;
	float cameraY;

	bool isHovered = false;

	int mouseX = 0;
	int mouseY = 0;
	int firstMouseX = 0;
	int firstMouseY = 0;
	bool isDragging = false;
	bool skipThisFrame = false;
};