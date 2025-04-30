#pragma once

#include <string>
#include <iostream>

#include "Node.h"

class PrintInput : public Input {
public:
	std::string text = "";
};

class Print : public Node
{
public:
	void Draw() override;
	void SetPos(int x, int y) override;
	std::function<void()> Compile() override;
	std::function<void()> Call(Input* input) override;
	char buf[32] = "";
	std::string GetType() override {
		return "Print";
	}
	int GetIdPass() override {
		return 4;
	}
	std::shared_ptr<Node> clone() override {
		return std::make_shared<Print>(*this);
	}
	void SetId(int id) override {
		this->id = id;
	}
	int GetId() override {
		return id;
	}
	nlohmann::json ToJson() override;
	bool FromJson(nlohmann::json json) override;
private:
	int id = 0;
	int x, y;
	bool first = true;
};

