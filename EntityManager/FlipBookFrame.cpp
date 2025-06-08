#include "pch.h"
#include "FlipBookFrame.h"

/*
PURPOSE: Creates the frame
*/
ENTITYMANAGER_API void FlipBookFrame::Create(int x, int y, int width, int height, float u, float v, float textureW, float textureH)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->u = u;
    this->v = v;
    this->textureWidth = textureW;
    this->textureHeight = textureH;

	//------ TEXTURE TILE ------
	//Vertices for the texture
	float tileVertices[] = {
		//      X		      Y			Z		 U	    		V
		 0.0f,		   0.0f,		  0.0f, u,            v,			// top right
		 0.0f,		   (float)height, 0.0f, u,            v + textureH,	// bottom right
		 (float)width, (float)height, 0.0f, u + textureW, v + textureH, // bottom left
		 (float)width, 0.0f,          0.0f, u + textureW, v,			// top left 
	};
	//Indices for the texture
	unsigned int tileIndices[] = {
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	//Create buffers and bind them with vertices and indices
	adr_glGenBuffers(1, &VBO);
	adr_glGenBuffers(1, &EBO);

	adr_glGenVertexArrays(1, &VAO);
	adr_glBindVertexArray(VAO);

	adr_glBindBuffer(GL_ARRAY_BUFFER, VBO);
	adr_glBufferData(GL_ARRAY_BUFFER, sizeof(tileVertices), tileVertices, GL_STATIC_DRAW);

	adr_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	adr_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tileIndices), tileIndices, GL_STATIC_DRAW);

	//Set the vertex attrib pointers (position = 0)
	adr_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	adr_glEnableVertexAttribArray(0);

	//Set the vertex attrib pointers (position = 1)
	adr_glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	adr_glEnableVertexAttribArray(1);

	//Release buffers
	adr_glBindBuffer(GL_ARRAY_BUFFER, 0);
	adr_glBindVertexArray(0);
}

/*
PURPOSE: Updates the frame
*/
ENTITYMANAGER_API void FlipBookFrame::Update()
{
}

/*
PURPOSE: Draws the frame
*/
ENTITYMANAGER_API void FlipBookFrame::Draw(int tileW, int tileH, glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale)
{
	//Set some transform
	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 tra = translate;
	float rot= 0.0f;
	glm::vec3 sca = scale;

	if (ShaderManager::GetInstance().GetCurrentType() == Utils::SHADER_3D) {
		tra /= 32.0f;
		sca /= 32.0f;
		rot = 180.0f;
	}

	model = glm::translate(model, tra);
	model = glm::rotate(model, glm::radians(rot + rotate.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotate.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotate.z), glm::vec3(0.0f, 0.0f, 1.0f));
	model = glm::scale(model, sca); //TODO: Change it with mouse scroll

	ShaderManager::GetInstance().ApplyTransformMatrix("uModel", model);

	//Draw the texture
	adr_glBindVertexArray(VAO);
	adr_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//Disable after drawing
	adr_glBindVertexArray(0);
}

/*
PURPOSE: Releases the frame
*/
ENTITYMANAGER_API void FlipBookFrame::Release()
{
	if (VAO != -1) {
		adr_glDeleteVertexArrays(1, &VAO);
		VAO = -1;
	}
	if (VBO != -1) {
		adr_glDeleteBuffers(1, &VBO);
		VBO = -1;
	}
	if (EBO != -1) {
		adr_glDeleteBuffers(1, &EBO);
		EBO = -1;
	}
}
