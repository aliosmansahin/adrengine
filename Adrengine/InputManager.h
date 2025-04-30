#pragma once

#include <GLFW/glfw3.h>
#include <unordered_map>
#include <iostream>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

class InputManager
{
public:
	bool InitEngine(GLFWwindow* window);
	void ReleaseEngine();
	bool IsKeyJustPressed(int key);
	bool IsKeyJustReleased(int key);
	bool IsKeyPressed(int key);
	bool IsMouseButtonJustPressed(int key);
	bool IsMouseButtonJustReleased(int key);
	bool IsMouseButtonPressed(int key);
	void SetMouseVisibility(bool visible);
	int GetMouseX();
	int GetMouseY();
	void SetMousePos(int x, int y);
	void Update();
	static void KeyCallback(GLFWwindow* /*win*/, int key, int /*scancode*/, int action, int /*mods*/);
	static void CursorPositionCallback(GLFWwindow* /*win*/, double xpos, double ypos);
	static void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	static InputManager& GetInstance();
private:
	InputManager() = default;
	~InputManager() = default;
	InputManager(const InputManager&) = delete;
	InputManager& operator=(const InputManager&) = delete;
private:
	GLFWwindow* window = nullptr;
	static std::unordered_map<int, bool> currentKeys;
	static std::unordered_map<int, bool> previousKeys;
	static std::unordered_map<int, bool> currentMouseButtons;
	static std::unordered_map<int, bool> previousMouseButtons;
	static int mouseX;
	static int mouseY;
	static bool mouseVisibility;
};