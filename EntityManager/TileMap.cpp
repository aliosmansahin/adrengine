#include "pch.h"
#include "TileMap.h"

/*
PURPOSE: Initializes the entity
*/
bool TileMap::CreateEntity(std::shared_ptr<EntityParams> params)
{
	//Cast EntityParams to Sprite2DParams to use its properties
	auto casted = std::dynamic_pointer_cast<TileMapParams>(params);
	if (!casted) {
		Logger::Log("E", "Casting failed at dynamic_cast<TileMapParams*>(params)");
		return false;
	}
	this->params = casted;

	return true;
}

/*
PURPOSE: Unitializes the entity
*/
void TileMap::DeleteEntity()
{
	for (auto& tile : createdTiles)
		tile.second->Release();
	createdTiles.clear();

	tiles.clear();
}

/*
PURPOSE: Updates the entity
*/
void TileMap::Update()
{
	//Tiles
	for (auto& tile : tiles) {
		tile.second->SetPos((int)(params->x + (float)tile.first.first), (int)(params->x + (float)tile.first.second));
	}
}

/*
PURPOSE: Draws the entity.
	This entity draws a texture
*/
void TileMap::Draw(glm::vec3 currentSceneCameraPos)
{
	//Tiles
	for (auto& tile : tiles) {
		tile.second->Draw(tileWidth, tileHeight, params->texture);
	}

	//Tile indicator
	if (tileIndicator.get() && drawTileIndicator) {
		//Set some stuff
		ShaderManager::GetInstance().ApplyUniformBool("drawTileIndicator", true);
		ShaderManager::GetInstance().ApplyUniformInt("tileWidth", tileWidth);
		ShaderManager::GetInstance().ApplyUniformInt("tileHeight", tileHeight);

		//Drawing
		tileIndicator->Draw(tileWidth, tileHeight, 0);

		//Disabling
		ShaderManager::GetInstance().ApplyUniformBool("drawTileIndicator", false);
		drawTileIndicator = false;
	}
}

/*
PURPOSE: Add a tile which is selected from createdTiles to the map
*/
ENTITYMANAGER_API void TileMap::AddTileToMap(TileMap* tileMap, int mouseX, int mouseY, float cameraX, float cameraY, std::pair<int, int> selectedTile)
{
	//Get the tile pos on the scene
	float tileXAtScene = (float)mouseX + cameraX;
	float tileYAtScene = (float)mouseY + cameraY;

	//Calculate the tile pos on the tileMap
	int tileX = (int)tileXAtScene / tileMap->tileWidth;
	int tileY = (int)tileYAtScene / tileMap->tileHeight;

	//Get the selected tile and insert it to tiles to draw
	auto createdTileIter = tileMap->createdTiles.find(selectedTile);
	if (createdTileIter == tileMap->createdTiles.end())
		return;

	//Check if there is a tile on this coordinates
	auto tileIter = tileMap->tiles.find({ tileX, tileY });

	//Add a tile if there is not
	if (tileIter == tileMap->tiles.end()) {
		std::shared_ptr<Tile> tile = std::make_shared<Tile>(*createdTileIter->second.get());
		tileMap->tiles.insert({ { tileX, tileY }, tile });
	}
}

/*
PURPOSE: Removes the tile which is located by mouse from the map
*/
ENTITYMANAGER_API void TileMap::RemoveTileFromMap(TileMap* tileMap, int mouseX, int mouseY, float cameraX, float cameraY)
{
	//Get the tile pos on the scene
	float tileXAtScene = (float)mouseX + cameraX;
	float tileYAtScene = (float)mouseY + cameraY;

	//Calculate the tile pos on the tileMap
	int tileX = (int)tileXAtScene / tileMap->tileWidth;
	int tileY = (int)tileYAtScene / tileMap->tileHeight;

	//Check if there is a tile on this coordinates
	auto tileIter = tileMap->tiles.find({ tileX, tileY });

	//Remove the tile if there is
	if (tileIter != tileMap->tiles.end()) {
		tileMap->tiles.erase(tileIter);
	}
}

/*
PURPOSE: Draws a rectange to indicate which tile coordinates will be filled
*/
void TileMap::UpdateMouseTileIndicator(TileMap* tileMap, int mouseX, int mouseY, float cameraX, float cameraY)
{
	//Get the tile pos on the scene
	float tileXAtScene = (float)mouseX + cameraX;
	float tileYAtScene = (float)mouseY + cameraY;

	//Calculate the tile pos on the tileMap
	int tileX = (int)(tileXAtScene / (float)tileMap->tileWidth);
	int tileY = (int)(tileYAtScene / (float)tileMap->tileHeight);

	//We will add 1 when tile pos is sub-zero
	if (tileXAtScene < 0.0f)
		tileX--;
	if (tileYAtScene < 0.0f)
		tileY--;

	//Set position of the tile indicator and activate to show
	if (tileMap->tileIndicator.get()) {
		tileMap->tileIndicator->SetPos(tileX, tileY);
		tileMap->drawTileIndicator = true;
	}
}

/*
PURPOSE: Creates each tile from the inspector
*/
ENTITYMANAGER_API void TileMap::CreateTiles(float textureWidth, float textureHeight, float tileWidth, float tileHeight)
{
	//Delete old tiles
	for (auto& tile : createdTiles)
		tile.second->Release();
	createdTiles.clear();

	this->tileWidth = (int)tileWidth;
	this->tileHeight = (int)tileHeight;

	int tileCountX = (int)std::ceil(textureWidth / tileWidth);
	int tileCountY = (int)std::ceil(textureHeight / tileHeight);
	
	for (int y = 0; y < tileCountY; ++y) {

		/*
			These calculations are for checking if some of tiles don't have decided size.
			It they don't we will give them as most size as possible,
			In this scenario, we will give them the last texture size.
		*/
		//pass tile height into a new variable
		int realTileHeight = (int)tileHeight;

		//if heights of the tiles aren't equal to each other
		if ((int)textureHeight % (int)tileHeight != 0) {
			//if this tile is the last in y coord in the tilemap
			if (y == tileCountY - 1) {
				//calculate y position of the tile in the tilemap and sub it from tilemap width
				int currentPosYInTileMap = y * (int)tileHeight;
				int lastTextureSizeY = (int)textureHeight - currentPosYInTileMap;

				//pass the new width
				realTileHeight = lastTextureSizeY;
			}
		}

		for (int x = 0; x < tileCountX; ++x) {
			//pass tile width into a new variable
			int realTileWidth = (int)tileWidth;

			//if widths of the tiles aren't equal to each other
			if ((int)textureWidth % (int)tileWidth != 0) {

				//if this tile is the last in x coord in the tilemap
				if (x == tileCountX - 1) {
					//calculate x position of the tile in the tilemap and sub it from tilemap width
					int currentPosXInTileMap = x * (int)tileWidth;
					int lastTextureSizeX = (int)textureWidth - currentPosXInTileMap;

					//pass the new width
					realTileWidth = lastTextureSizeX;
				}
			}
			
			//Create tile and insert it into createdTiles
			Tile* tile = new Tile();
			float texturePosX = x * tileWidth;
			float texturePosY = y * tileHeight;
			
			/*
				Here we are translating pixel coordinates to[0.0, 1.0].
				For example, the texture width is 512 pixels, and the current tile pos is 256,
				After translation, the new x value will be 0.5
			*/
			float u = texturePosX * 1.0f / textureWidth;
			float v = texturePosY * 1.0f / textureHeight;

			//TT* stands for "translated tile width/height" btw
			float TTX = realTileWidth * 1.0f / textureWidth;
			float TTY = realTileHeight * 1.0f / textureHeight;

			tile->Create(x, y, realTileWidth, realTileHeight, u, v, TTX, TTY);

			createdTiles.insert({ { x, y }, std::shared_ptr<Tile>(tile) });

			//std::cout << "x: " << x << " " << " y: " << y << std::endl;
			//std::cout << "u " << u << " " << u + TTX << " v " << v << " " << v + TTY << std::endl;
			//std::cout << "x: " << realTileWidth << " y: " << realTileHeight << std::endl;
		}
	}

}

/*
PURPOSE: Creates a framebuffer and other buffer objects for the inspector
*/
ENTITYMANAGER_API void TileMap::CreateInspectFrameBuffer(float width, float height, int tileW, int tileH, float scale)
{
	//Delete old tiles
	for (auto& tile : createdTiles)
		tile.second->Release();
	createdTiles.clear();

	width *= scale;
	height *= scale;

	//------ RELEASING ------
	
	//Delete old Buffers
	if (inspectFrameBuffer != -1) {
		glDeleteFramebuffers(1, &inspectFrameBuffer);
		inspectFrameBuffer = -1;
	}
	if (inspectRenderBuffer != -1) {
		glDeleteRenderbuffers(1, &inspectRenderBuffer);
		inspectRenderBuffer = -1;
	}
	if (inspectTexture != -1) {
		glDeleteTextures(1, &inspectTexture);
		inspectTexture = -1;
	}
	if (VAO != -1) {
		glDeleteVertexArrays(1, &VAO);
		VAO = -1;
	}
	if (VBO != -1) {
		glDeleteBuffers(1, &VBO);
		VBO = -1;
	}
	if (EBO != -1) {
		glDeleteBuffers(1, &EBO);
		EBO = -1;
	}
	if (tileIndicator.get())
		tileIndicator->Release();

	//------ TILE POS INDICATOR ------

	tileIndicator = std::make_shared<Tile>();
	tileIndicator->Create(0, 0, tileW, tileH, 0.0f, 0.0f, 1.0f, 1.0f);

	//------ TEXTURE TILEMAP ------

	//Vertices for the texture
	float vertices[] = {
		//X		Y		Z	 U     V
		 width, height, 0.0f, 1.0f, 0.0f, // top right
		 0.0f,	height, 0.0f, 0.0f, 0.0f, // bottom right
		 0.0f,  0.0f,	0.0f, 0.0f, 1.0f, // bottom left
		 width, 0.0f,   0.0f, 1.0f, 1.0f, // top left 
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

	//------ FRAME BUFFER ------

	//frame buffer
	glGenFramebuffers(1, &inspectFrameBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, inspectFrameBuffer);

	//generate a texture and bind it to frame buffer
	glGenTextures(1, &inspectTexture);
	glBindTexture(GL_TEXTURE_2D, inspectTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)width, (GLsizei)height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, inspectTexture, 0);

	//check the status of frame buffer
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Logger::Log("E", "Framebuffer is not complete");

	//generate render buffer and bind it to the frame buffer
	glGenRenderbuffers(1, &inspectRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, inspectRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)width, (GLsizei)height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, inspectRenderBuffer);

	//release all buffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

/*
PURPOSE: Updates the inspector, its empty for now
*/
ENTITYMANAGER_API void TileMap::UpdateInspect()
{
}

/*
PURPOSE: Renders the inspector to the framebuffer
*/
ENTITYMANAGER_API void TileMap::DrawInspect(int width, int height, int tileW, int tileH, float scale)
{
	//Save the previous type of shader
	Utils::ShaderType prevShaderType = ShaderManager::GetInstance().GetCurrentType();

	//Change the shader to draw inspector
	ShaderManager::GetInstance().UseShaders(Utils::SHADER_INSPECT_TILE);

	//Set the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, inspectFrameBuffer);
	Graphics::GetInstance().Clear();
	glViewport(0, 0, (GLsizei)(width * scale), (GLsizei)(height * scale));

	//Some calculations for transformation
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(scale, scale, 1.0f));
	glm::mat4 proj = glm::ortho(0.0f, (float)width * scale, (float)height * scale, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);

	//Send the transformation matrix to the shader
	ShaderManager::GetInstance().ApplyTransformMatrix("uModel", model);
	ShaderManager::GetInstance().ApplyTransformMatrix("uProjection", proj);
	ShaderManager::GetInstance().ApplyTransformMatrix("uView", view);
	ShaderManager::GetInstance().ApplyUniformInt("tileW", tileW);
	ShaderManager::GetInstance().ApplyUniformInt("tileH", tileH);
	ShaderManager::GetInstance().ApplyUniformInt("textureWidth", width);
	ShaderManager::GetInstance().ApplyUniformInt("textureHeight", height);

	//Set the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, params->texture);
	ShaderManager::GetInstance().ApplyTexture("texture1");

	//Draw the texture
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Disable after drawing
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindVertexArray(0);

	ShaderManager::GetInstance().UseShaders(prevShaderType);
}

/*
PURPOSE: Check if buffers for inspector are created
*/
ENTITYMANAGER_API bool TileMap::IsInspectCreated()
{
	if (inspectFrameBuffer == -1)
		return false;
	if (inspectRenderBuffer == -1)
		return false;
	if (inspectTexture == -1)
		return false;
	if (VAO == -1)
		return false;
	if (VBO == -1)
		return false;
	if (EBO == -1)
		return false;
	if (!tileIndicator.get())
		return false;
	return true;
}

/*
PURPOSE: Gets the texture of the inspector
*/
ENTITYMANAGER_API unsigned int TileMap::GetInspectTexture()
{
	return inspectTexture;
}

/*
PURPOSE: Returns createdTiles
*/
ENTITYMANAGER_API std::map<std::pair<int, int>, std::shared_ptr<Tile>>& TileMap::GetCreatedTiles()
{
	return createdTiles;
}

/*
PURPOSE: Returns properties of the entity as a pure pointer
*/
EntityParams* TileMap::GetEntityParams()
{
	return params.get();
}

/*
PURPOSE: Allows us to create json content of properties which belongs to type of the entity and return it.
	"Entity" creates entity json, "Object" creates object json.
*/
nlohmann::json TileMap::ToJson()
{
	nlohmann::json j; //For the entity

	if (params) {
		j = params->ToJson();
	}

	nlohmann::json c; //For created tiles
	for (auto& tile : createdTiles) {
		c.push_back(tile.second->ToJson());
	}
	j["created-tiles"] = c;

	nlohmann::json t; //For drawing tiles
	for (auto& tile : tiles) {
		t.push_back(tile.second->ToJson());
	}
	j["tiles"] = t;

	//Tile size
	j["tile-width"] = tileWidth;
	j["tile-height"] = tileHeight;

	return j;

}

/*
PURPOSE: TileMap has a fromjson function to load existing brush tiles and drawing tiles from json, it also creates tileIndicator object
*/
ENTITYMANAGER_API void TileMap::FromJson(nlohmann::json json)
{
	//Tiles to brush
	if (json.contains("created-tiles")) {
		nlohmann::json c = json["created-tiles"];
		for(auto& t : c) {
			Tile* tile = new Tile();
			tile->Create(
				t.value("x", 0),
				t.value("y", 0),
				t.value("width", 0),
				t.value("height", 0),
				t.value("u", 0.0f),
				t.value("v", 0.0f),
				t.value("tex-w", 0.0f),
				t.value("tex-h", 0.0f)
			); // We don't have to have a "fromjson" function, "create" handles it

			createdTiles.insert({ { tile->x, tile->y }, std::shared_ptr<Tile>(tile) });
		}
	}

	//Tiles to draw
	if (json.contains("tiles")) {
		nlohmann::json tileJson = json["tiles"];
		for (auto& t : tileJson) {
			Tile* tile = new Tile();
			tile->Create(
				t.value("x", 0),
				t.value("y", 0),
				t.value("width", 0),
				t.value("height", 0),
				t.value("u", 0.0f),
				t.value("v", 0.0f),
				t.value("tex-w", 0.0f),
				t.value("tex-h", 0.0f)
			); // We don't have to have a "fromjson" function, "create" handles it

			tiles.insert({ { tile->x, tile->y }, std::shared_ptr<Tile>(tile) });
		}
	}

	// Tile size
	tileWidth = json.value("tile-width", 0);
	tileHeight = json.value("tile-height", 0);

	//Tile indicator
	tileIndicator = std::make_shared<Tile>();
	tileIndicator->Create(0, 0, tileWidth, tileHeight, 0.0f, 0.0f, 1.0f, 1.0f);
}
