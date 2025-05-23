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

//Abstrack input class for the node
class VISUALSCRIPTMANAGER_API Input {
public:
	virtual ~Input() = default;
};

//Abstrack ode class
class Node
{
public:
	VISUALSCRIPTMANAGER_API virtual void			      Draw()						= 0;
	VISUALSCRIPTMANAGER_API virtual void			      SetPos(int x, int y)			= 0;
	VISUALSCRIPTMANAGER_API virtual std::function<void()> Compile()						= 0;
	VISUALSCRIPTMANAGER_API virtual std::function<void()> Call(Input* input)			= 0;
	VISUALSCRIPTMANAGER_API virtual std::string			  GetType()						= 0;
	VISUALSCRIPTMANAGER_API virtual int					  GetIdPass()					= 0;
	VISUALSCRIPTMANAGER_API virtual std::shared_ptr<Node> clone()						= 0;
	VISUALSCRIPTMANAGER_API virtual void				  SetId(int id)					= 0;
	VISUALSCRIPTMANAGER_API virtual int					  GetId()						= 0;
	VISUALSCRIPTMANAGER_API virtual nlohmann::json		  ToJson()						= 0;
	VISUALSCRIPTMANAGER_API virtual bool				  FromJson(nlohmann::json json) = 0;
};