#include "pch.h"
#include "Tile.h"

/*
PURPOSE: Creates the tile buffers.
	Parameters: x coordinate, y coordinate, width in pixels, height in pixels, x coord in texture, y coord of texture, texture width, texture height
*/
void Tile::Create(int x, int y, int width, int height, float u, float v, float textureW, float textureH)
{
	//Setup parameters
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
	glGenBuffers(1, &tileVBO);
	glGenBuffers(1, &tileEBO);

	glGenVertexArrays(1, &tileVAO);
	glBindVertexArray(tileVAO);

	glBindBuffer(GL_ARRAY_BUFFER, tileVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(tileVertices), tileVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tileEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(tileIndices), tileIndices, GL_STATIC_DRAW);

	//Set the vertex attrib pointers (position = 0)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//Set the vertex attrib pointers (position = 1)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//Release buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

/*
PURPOSE: Updates the tile
*/
void Tile::Update()
{
}

/*
PURPOSE: Draws the tile
*/
void Tile::Draw(int tileW, int tileH, unsigned int tileMap)
{
	//Set some transform
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(x * tileW, y * tileH, 0.0f));
	ShaderManager::GetInstance().ApplyTransformMatrix("uModel", model);

	//Set the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tileMap);
	ShaderManager::GetInstance().ApplyTexture("texture1");

	//Draw the texture
	glBindVertexArray(tileVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//Disable after drawing
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);
}

/*
PURPOSE: Releases the tile (buffers and other stuff)
*/
void Tile::Release()
{
	if (tileVAO != -1) {
		glDeleteVertexArrays(1, &tileVAO);
		tileVAO = -1;
	}
	if (tileVBO != -1) {
		glDeleteBuffers(1, &tileVBO);
		tileVBO = -1;
	}
	if (tileEBO != -1) {
		glDeleteBuffers(1, &tileEBO);
		tileEBO = -1;
	}
}

/*
PURPOSE: Sets the position of the tile
*/
void Tile::SetPos(int x, int y)
{
	this->x = x;
	this->y = y;
}
