#pragma once

#include "Logger.h"
#include <glad/glad.h>

class Entity
{
public:
	//main funcs
	~Entity();
	bool CreateEntity();
	void DeleteEntity();
	void Update();
	void Draw();
private:
	unsigned int VBO;
	unsigned int VAO;
	unsigned int EBO;
};

