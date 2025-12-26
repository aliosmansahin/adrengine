#pragma once

#include <memory>
#include <string>
#include <unordered_map>

#include "../IEntity/IEntity.h"

struct GLFWwindow;
struct ImGuiContext;
struct ImNodesContext;

class IProject
{
public:
	virtual ~IProject() = default;

	//Handle project
	virtual bool CreateProject(
		std::string projectPath,
		std::string projectName,
		GLFWwindow* window,
		ImGuiContext* context,
		ImNodesContext* nodesContext
	) = 0;
	virtual bool OpenProject(
		std::string& projectPath,
		std::string& projectName,
		GLFWwindow* window,
		ImGuiContext* context,
		ImNodesContext* nodesContext
	) = 0;
	virtual bool CloseProject() = 0;
	virtual bool SaveProject() = 0;

	//Getters
	virtual std::string& GetProjectFileLocation() = 0;
	virtual std::string& GetProjectDir() = 0;

	//Latest projects
	virtual void LoadLatestProjects() = 0;
	virtual void AddProjectToLatestProjects(std::string& projectFilepath) = 0;
	virtual void RemoveProjectFromLatestProjects(std::string& projectFilepath) = 0;
	virtual void RemoveProjectFromLatestProjectsByIndex(int& index) = 0;
	virtual void MoveProjectToBegin(std::string& projectFilepath) = 0;
	virtual void MoveProjectToBeginByIndex(int& index, std::string& projectFilepathWillBeAdded) = 0;
	virtual void SaveLatestProjects() = 0;
	virtual std::vector<std::string>& GetLatestProjects() = 0;
	virtual bool GetProjectOpened() = 0;
};