#pragma once

#ifdef INPUTMANAGER_EXPORTS
#define INPUTMANAGER_API __declspec(dllexport)
#else
#define INPUTMANAGER_API __declspec(dllimport)
#endif

#include "glfw/glfw3.h"
#include <unordered_map>
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"

class InputManager
{
public:
	//main functions
	INPUTMANAGER_API bool				  InitEngine(GLFWwindow* window, ImGuiContext* context);
	INPUTMANAGER_API void				  ReleaseEngine();
	INPUTMANAGER_API void				  Update(GLFWwindow* window);

	//keys
	INPUTMANAGER_API bool				  IsKeyJustPressed(int key);
	INPUTMANAGER_API bool				  IsKeyJustReleased(int key);
	INPUTMANAGER_API bool				  IsKeyPressed(int key);

	//mouse buttons
	INPUTMANAGER_API bool				  IsMouseButtonJustPressed(int key);
	INPUTMANAGER_API bool				  IsMouseButtonJustReleased(int key);
	INPUTMANAGER_API bool				  IsMouseButtonPressed(int key);

	//mouse visibility
	INPUTMANAGER_API bool				  GetMouseVisibility();
	INPUTMANAGER_API void				  SetMouseVisibility(bool visibility);

	//getter for mouse positions
	INPUTMANAGER_API int				  GetMouseX();
	INPUTMANAGER_API int				  GetMouseY();

	//setter for mouse positions
	INPUTMANAGER_API void				  SetMousePos(int x, int y);

	//getter for the instance
	INPUTMANAGER_API static InputManager& GetInstance();
private:
	//singleton
	InputManager() = default;
	~InputManager() = default;
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
private:
	//GLFW
	GLFWwindow* window = nullptr;

	//maps for keys and mouse buttons
	INPUTMANAGER_API static std::unordered_map<int, bool> currentKeys;
	INPUTMANAGER_API static std::unordered_map<int, bool> previousKeys;
	INPUTMANAGER_API static std::unordered_map<int, bool> currentMouseButtons;
	INPUTMANAGER_API static std::unordered_map<int, bool> previousMouseButtons;

	//mouse status
	INPUTMANAGER_API static int mouseX;
	INPUTMANAGER_API static int mouseY;
	INPUTMANAGER_API static bool mouseVisibility;
};