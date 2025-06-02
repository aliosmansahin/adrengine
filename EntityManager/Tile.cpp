#include "pch.h"
#include "Tile.h"

/*
PURPOSE: Creates the tile buffers.
	Parameters: x coordinate, y coordinate, width in pixels, height in pixels, x coord in texture, y coord of texture, texture width, texture height
*/
void Tile::Create(int x, int y, int width, int height, float u, float v, float textureW, float textureH, std::pair<int, int> tileType)
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
	this->tileType = tileType;

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
	adr_glGenBuffers(1, &tileVBO);
	adr_glGenBuffers(1, &tileEBO);

	adr_glGenVertexArrays(1, &tileVAO);
	adr_glBindVertexArray(tileVAO);

	adr_glBindBuffer(GL_ARRAY_BUFFER, tileVBO);
	adr_glBufferData(GL_ARRAY_BUFFER, sizeof(tileVertices), tileVertices, GL_STATIC_DRAW);

	adr_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tileEBO);
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
PURPOSE: Updates the tile
*/
void Tile::Update()
{
}

/*
PURPOSE: Draws the tile
*/
void Tile::Draw(int tileW, int tileH, int translateX, int translateY, int translateZ)
{
	//Set some transform
	glm::mat4 model = glm::mat4(1.0f);

	glm::vec3 tra = glm::vec3(x * tileW + translateX, y * tileH + translateY, translateZ);
	float rotate = 0.0f;
	glm::vec3 sca = glm::vec3(1.0f);

	if (ShaderManager::GetInstance().GetCurrentType() == Utils::SHADER_3D) {
		tra /= 32.0f;
		sca /= 32.0f;
		rotate = 180.0f;
	}

	model = glm::translate(model, tra);
	model = glm::rotate(model, glm::radians(rotate), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::scale(model, sca); //TODO: Change it with mouse scroll

	ShaderManager::GetInstance().ApplyTransformMatrix("uModel", model);
	
	//Draw the texture
	adr_glBindVertexArray(tileVAO);
	adr_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//Disable after drawing
	adr_glBindVertexArray(0);
}

/*
PURPOSE: Releases the tile (buffers and other stuff)
*/
void Tile::Release()
{
	if (tileVAO != -1) {
		adr_glDeleteVertexArrays(1, &tileVAO);
		tileVAO = -1;
	}
	if (tileVBO != -1) {
		adr_glDeleteBuffers(1, &tileVBO);
		tileVBO = -1;
	}
	if (tileEBO != -1) {
		adr_glDeleteBuffers(1, &tileEBO);
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

nlohmann::json Tile::ToJson()
{
	nlohmann::json j;
	j["type-x"] = tileType.first;
	j["type-y"] = tileType.second;

	//position and size of the tile
	j["x"] = x;
	j["y"] = y;
	j["width"] = width;
	j["height"] = height;

	//position and size of the texture
	j["u"] = u;
	j["v"] = v;
	j["tex-w"] = textureWidth;
	j["tex-h"] = textureHeight;
	return j;
}
