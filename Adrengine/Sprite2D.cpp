#include "Sprite2D.h"

bool Sprite2D::CreateEntity(std::shared_ptr<EntityParams> params)
{
	auto casted = std::dynamic_pointer_cast<Sprite2DParams>(params);
	if (!casted) {
		Logger::Log("E", "Casting failed at dynamic_cast<Sprite2DParams*>(params)");
		return false;
	}
	this->params = casted;

	//some vertices
	float vertices[] = {
		 0.0f,  32.0f, 0.0f, 1.0f, 1.0f, // top right
		 32.0f, 32.0f, 0.0f, 1.0f, 0.0f, // bottom right
		 32.0f, 0.0f,  0.0f, 0.0f, 0.0f, // bottom left
		 0.0f,  0.0f,  0.0f, 0.0f, 1.0f, // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	//create buffers and bind them with vertices
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//set the vertex attrib pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//set the vertex attrib pointers
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//release buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return true;
}

void Sprite2D::DeleteEntity()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

void Sprite2D::Update()
{
}

void Sprite2D::Draw()
{
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(params->x, params->y, params->z));
	model = glm::rotate(model, glm::radians(params->rx), glm::vec3(1.0, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(params->ry), glm::vec3(0.0, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(params->rz), glm::vec3(0.0, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(params->sx, params->sy, params->sz));

	ShaderManager::GetInstance().ApplyTransformMatrix("uModel", model);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, params->texture);
	ShaderManager::GetInstance().ApplyTexture();
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

EntityParams* Sprite2D::GetEntityParams()
{
	return params.get();
}

nlohmann::json Sprite2D::ToJson()
{
	nlohmann::json j;

	if (params) {
		j = params->ToJson();  // EntityParams'ý JSON'a çevir
	}
	return j;

}
