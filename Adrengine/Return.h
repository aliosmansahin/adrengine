#pragma once

#include "Node.h"

class Return : public Node
{
public:
	void Draw() override;
	void SetPos(int x, int y) override;
	std::function<void()> Compile() override;
	std::function<void()> Call(Input* input) override;
	std::string GetType() override {
		return "Return";
	}
	int GetIdPass() override {
		return 0;
	}
	std::shared_ptr<Node> clone() override {
		return std::make_shared<Return>(*this);
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

