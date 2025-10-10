#pragma once

#ifdef PROJECT_EXPORTS
#define PROJECT_API __declspec(dllexport)
#else
#define PROJECT_API __declspec(dllimport)
#endif

#include <string>
#include <fstream>

#include <Logger.h>
#include <AssetDatabase.h>
#include <AssetSaver.h>
#include <InputManager.h>

#include <nlohmann_json/json.hpp>

#include "interfaces/ISceneManager/ISceneManager.h"

#include "interfaces/IEntity/IEntity.h"

#include "interfaces/IVisualScriptManager/IVisualScriptManager.h"

#include "interfaces/IProject/IProject.h"

#include "ServiceLocator.h"

class Project : public IProject
{
private:
	//Singleton
	Project() = default;
	~Project() = default;
	Project(const Project&) = delete;
	Project& operator=(const Project&) = delete;

public:
	static Project& GetInstance() {
		static Project instance;
		return instance;
	}

public:
	//main
	PROJECT_API bool CreateProject(
		std::string projectPath,
		std::string projectName,
		GLFWwindow* window,
		ImGuiContext* context,
		ImNodesContext* nodesContext
	) override;
	PROJECT_API bool OpenProject(
		std::string& projectPath,
		std::string& projectName,
		GLFWwindow* window,
		ImGuiContext* context,
		ImNodesContext* nodesContext
	) override;
	PROJECT_API bool CloseProject() override;
	PROJECT_API bool SaveProject() override;

	//getters
	PROJECT_API std::string& GetProjectFileLocation() override;
	PROJECT_API std::string& GetProjectDir() override;

	//latest projects
	PROJECT_API void LoadLatestProjects() override;
	PROJECT_API void AddProjectToLatestProjects(std::string& projectFilepath) override;
	PROJECT_API void RemoveProjectFromLatestProjects(std::string& projectFilepath) override;
	PROJECT_API void RemoveProjectFromLatestProjectsByIndex(int& index) override;
	PROJECT_API void MoveProjectToBegin(std::string& projectFilepath) override;
	PROJECT_API void MoveProjectToBeginByIndex(int& index, std::string& projectFilepathWillBeAdded) override;
	PROJECT_API void SaveLatestProjects() override;
	PROJECT_API std::vector<std::string>& GetLatestProjects() override;
	PROJECT_API bool GetProjectOpened() override;

private:
	//Is a project opened
	bool projectOpened = false;

private:
	//project specifications
	std::string projectName = "";
	std::string projectPath = "";
	std::string projectDir = "";
	std::string projectFileLocation = "";

	//Stores latest projects file paths
	std::vector<std::string> latestProjects;
};