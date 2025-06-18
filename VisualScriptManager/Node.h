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
	VISUALSCRIPTMANAGER_API virtual ~Node()												= default;
	VISUALSCRIPTMANAGER_API virtual void			      Draw(NodeVisual* nodeVisual)	= 0;
	VISUALSCRIPTMANAGER_API virtual void			      SetPos(int x, int y)			= 0;
	VISUALSCRIPTMANAGER_API virtual void			      SetPins()						= 0;
	VISUALSCRIPTMANAGER_API virtual void				  Execute()						= 0;
	VISUALSCRIPTMANAGER_API virtual Value				  Evaluate(Pin* pin) {
		return std::monostate{};
	};
	VISUALSCRIPTMANAGER_API	Node* GetNextExecNode(Pin* execOutputPin) {
		if (execOutputPin->connectedTo)
			return execOutputPin->connectedTo->parentNode;
		return nullptr;
	}
	VISUALSCRIPTMANAGER_API virtual std::string			  GetType()						= 0;
	VISUALSCRIPTMANAGER_API virtual std::shared_ptr<Node> clone()						= 0;
	VISUALSCRIPTMANAGER_API virtual nlohmann::json		  ToJson()						= 0;
	VISUALSCRIPTMANAGER_API virtual bool				  FromJson(nlohmann::json json) = 0;
	std::vector<std::shared_ptr<Pin>> inputPins;
	std::vector<std::shared_ptr<Pin>> outputPins;
	std::string title;
};