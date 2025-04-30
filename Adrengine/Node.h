#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "imnodes/imnodes.h"

#include <functional>
#include <string>
#include <memory>

class Input {
public:
	virtual ~Input() = default;
};

class Node
{
public:
	virtual void Draw() = 0;
	virtual void SetPos(int x, int y) = 0;
	virtual std::function<void()> Compile() = 0;
	virtual std::function<void()> Call(Input* input) = 0;
	virtual std::string GetType() = 0;
	virtual int GetIdPass() = 0;
	virtual std::shared_ptr<Node> clone() = 0;
	virtual void SetId(int id) = 0;
	virtual int GetId() = 0;
};