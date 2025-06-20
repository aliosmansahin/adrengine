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

struct NodeVisual {
	std::shared_ptr<Node> logicNode;
	int id;
	std::vector<int> inputIds;
	std::vector<int> outputIds;
};

class Node
{
public:
	VISUALSCRIPTMANAGER_API virtual ~Node()	= default;

	//Drawing
	VISUALSCRIPTMANAGER_API void			BeginDraw(NodeVisual* nodeVisual);
	VISUALSCRIPTMANAGER_API virtual void	Draw(NodeVisual* nodeVisual);
	VISUALSCRIPTMANAGER_API void			EndDraw();

	//Setters
	VISUALSCRIPTMANAGER_API void			SetPos(int x, int y);
	VISUALSCRIPTMANAGER_API virtual void	SetPins() = 0;

	//Execution
	VISUALSCRIPTMANAGER_API virtual void	Execute();
	VISUALSCRIPTMANAGER_API virtual Value	Evaluate(Pin* pin);
	VISUALSCRIPTMANAGER_API Value			EvaluateInput(Pin* pin);
	VISUALSCRIPTMANAGER_API Node*			GetNextExecNode(Pin* execOutputPin);

	//Getters
	VISUALSCRIPTMANAGER_API virtual std::string	GetType() = 0;

	//Clone
	VISUALSCRIPTMANAGER_API virtual std::shared_ptr<Node> clone() = 0;

	//Json
	VISUALSCRIPTMANAGER_API virtual nlohmann::json	ToJson();
	VISUALSCRIPTMANAGER_API virtual bool			FromJson(nlohmann::json json);
public:
	//pins
	std::vector<std::shared_ptr<Pin>> inputPins;
	std::vector<std::shared_ptr<Pin>> outputPins;
protected:
	//variables
	int x = 0, y = 0;
	bool first = true;
};