#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "Logger.h"

class InterfaceManager
{
public:
	~InterfaceManager();
	bool InitInterface(GLFWwindow* window);
	void CloseInterface();
	void StartFrame();
	void EndFrame();
	void DrawInterface();
	bool showDemoFrame = true;
};