#pragma once

/*
	Node is a parent.
	Other nodes inherits from that

	Some nodes have an input or output, or both of them, or none of them
*/

#ifdef VISUALSCRIPTMANAGER_EXPORTS
#define VISUALSCRIPTMANAGER_API __declspec(dllexport)
#else
#define VISUALSCRIPTMANAGER_API __declspec(dllimport)
#endif

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "imnodes/imnodes.h"

#include <functional>
#include <string>
#include <memory>

#include "nlohmann_json/json.hpp"

#include "Pin.h"
#include "interfaces/IVisualScript/INode/INode.h"

class Node : public INode, public std::enable_shared_from_this<Node>
{
public:
	VISUALSCRIPTMANAGER_API virtual ~Node()	= default;

	//Drawing
	VISUALSCRIPTMANAGER_API void			BeginDraw(std::shared_ptr<NodeVisual> nodeVisual) override;
	VISUALSCRIPTMANAGER_API virtual void	Draw(std::shared_ptr<NodeVisual> nodeVisual) override;
	VISUALSCRIPTMANAGER_API void			EndDraw() override;

	//Setters
	VISUALSCRIPTMANAGER_API void			SetPos(int x, int y) override;
	VISUALSCRIPTMANAGER_API virtual void	SetPins() = 0;

	//Execution
	VISUALSCRIPTMANAGER_API virtual void	Execute() override;
	VISUALSCRIPTMANAGER_API virtual Value	Evaluate(std::shared_ptr<IPin> pin);
	VISUALSCRIPTMANAGER_API Value			EvaluateInput(std::shared_ptr<IPin> pin);
	VISUALSCRIPTMANAGER_API std::shared_ptr<INode> GetNextExecNode(std::shared_ptr<IPin> execOutputPin);

	//Getters
	VISUALSCRIPTMANAGER_API virtual std::string	GetType() = 0;
	VISUALSCRIPTMANAGER_API std::vector<std::shared_ptr<IPin>>& GetInputPins() override {
		return inputPins;
	};
	VISUALSCRIPTMANAGER_API std::vector<std::shared_ptr<IPin>>& GetOutputPins() override {
		return outputPins;
	};

	//Clone
	VISUALSCRIPTMANAGER_API virtual std::shared_ptr<INode> clone() = 0;

	//Json
	VISUALSCRIPTMANAGER_API virtual nlohmann::json	ToJson() override;
	VISUALSCRIPTMANAGER_API virtual bool			FromJson(nlohmann::json json) override;

protected:
	//pins
	std::vector<std::shared_ptr<IPin>> inputPins;
	std::vector<std::shared_ptr<IPin>> outputPins;
protected:
	//variables
	int x = 0, y = 0;
	bool first = true;
};