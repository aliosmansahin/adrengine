#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"

#include "MenuBar.h"

class InterfaceManager
{
public:
	//main funcs
	bool InitInterface(GLFWwindow* window);
	void CloseInterface();
	//drawing
	void StartFrame();
	void EndFrame();
	void DrawInterface();
public:
	//getter for the instance
	static InterfaceManager& GetInstance();
private:
	//singleton
	InterfaceManager() = default;
	~InterfaceManager() = default;
	InterfaceManager(const InterfaceManager&) = delete;
	InterfaceManager& operator=(const InterfaceManager&) = delete;
};