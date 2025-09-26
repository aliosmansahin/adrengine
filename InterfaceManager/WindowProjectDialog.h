#pragma once

#ifdef INTERFACEMANAGER_EXPORTS
#define INTERFACEMANAGER_API __declspec(dllexport)
#else
#define INTERFACEMANAGER_API __declspec(dllimport)
#endif

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "imguifiledialog/ImGuiFileDialog.h"

#include <filesystem>

#include "Logger.h"

class WindowProjectDialog
{
public:
	//functions
	INTERFACEMANAGER_API void						 DrawWindow(std::vector<std::string>& latestProjects);
	INTERFACEMANAGER_API static WindowProjectDialog& GetInstance();
private:
	//singleton
	WindowProjectDialog() = default;
	~WindowProjectDialog() = default;
	WindowProjectDialog(const WindowProjectDialog&) = delete;
	WindowProjectDialog& operator=(const WindowProjectDialog&) = delete;
public:
	//window
	bool showWindow = true;

	//turns true when creating or opening a project
	bool isCreatingProject = false;
	bool isOpeningProject = false;

	//project spesifications
	std::string createPath = "";
	std::string createProjectName = "";
	std::string openPath = "";
	std::string openProjectName = "";
};

