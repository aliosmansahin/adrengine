#include "pch.h"
#include "Sprite2D.h"
#include "ShaderManager.h"

/*
PURPOSE: Initializes the entity
*/
bool Sprite2D::CreateEntity(std::shared_ptr<EntityParams> params)
{
	//Cast EntityParams to Sprite2DParams to use its properties
	auto casted = std::dynamic_pointer_cast<Sprite2DParams>(params);
	if (!casted) {
		Logger::Log("E", "Casting failed at dynamic_cast<Sprite2DParams*>(params)");
		return false;
	}
	this->params = casted;

	//Vertices for the texture
	float vertices[] = {
		//X     Y      Z	 U     V
		 32.0f, 32.0f, 0.0f, 1.0f, 0.0f, // top right
		 0.0f,  32.0f, 0.0f, 0.0f, 0.0f, // bottom right
		 0.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom left
		 32.0f, 0.0f,  0.0f, 1.0f, 1.0f, // top left 
	};
	//Indices for the texture
	unsigned int indices[] = {
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	//Create buffers and bind them with vertices and indices
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Set the vertex attrib pointers (position = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Set the vertex attrib pointers (position = 1)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Release buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return true;
}

/*
PURPOSE: Unitializes the entity
*/
void Sprite2D::DeleteEntity()
{
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteVertexArrays(1, &VAO);
}

/*
PURPOSE: Updates the entity
*/
void Sprite2D::Update()
{	
	/*
		Here we are calculating the real position, rotation and scale from properties.
		We will use these vectors to draw it in the space.
		If the entity has a parent, add the paramters to real* which is currently from parent's real*.
		Otherwise real* will be properties directly
	*/
	if (params.get()) {
		if (params->parent.get()) {
			realPos += glm::vec3(params->x, params->y, params->z);
			realRot += glm::vec3(params->rx, params->ry, params->rz);
			realSca *= glm::vec3(params->sx, params->sy, params->sz);
		}
		else {
			realPos = glm::vec3(params->x, params->y, params->z);
			realRot = glm::vec3(params->rx, params->ry, params->rz);
			realSca = glm::vec3(params->sx, params->sy, params->sz);
		}
	}
}

/*
PURPOSE: Draws the entity.
	This entity draws a texture
*/
void Sprite2D::Draw(glm::vec3 currentSceneCameraPos)
{
	//Some calculations for transformation
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, realPos);
	model = glm::rotate(model, glm::radians(realRot.x), glm::vec3(1.0, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(realRot.y), glm::vec3(0.0, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(realRot.z), glm::vec3(0.0, 0.0f, 1.0f));
	model = glm::scale(model, realSca);

	//Send the transformation matrix to the shader
	ShaderManager::GetInstance().ApplyTransformMatrix("uModel", model);

	//Set the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, params->texture);
	ShaderManager::GetInstance().ApplyTexture("texture1");

	//Draw the texture
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//Disable after drawing
	glActiveTexture(0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

/*
PURPOSE: Returns properties of the entity as a pure pointer
*/
EntityParams* Sprite2D::GetEntityParams()
{
	return params.get();
}

/*
PURPOSE: Allows us to create json content of properties which belongs to type of the entity and return it.
	"Entity" creates entity json, "Object" creates object json.
*/
nlohmann::json Sprite2D::ToJson()
{
	nlohmann::json j;

	if (params) {
		j = params->ToJson();
	}
	return j;

}
