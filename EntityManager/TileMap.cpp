#include "pch.h"
#include "TileMap.h"

/*
PURPOSE: Initializes the entity
*/
bool TileMap::CreateEntity(std::shared_ptr<EntityParams> params)
{
	//Cast EntityParams to TileMapParams to use its properties
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
		tile.second->SetPos(tile.first.first, tile.first.second);
	}
}

/*
PURPOSE: Draws the entity.
	This entity draws tiles and an indicator to know which tile will be filled
*/
void TileMap::Draw(glm::vec3 currentSceneCameraPos)
{
	//Set the texture for tiles
	adr_glActiveTexture(GL_TEXTURE0);
	adr_glBindTexture(GL_TEXTURE_2D, params->texture);

	//Shader uniforms
	if (ShaderManager::GetInstance().GetCurrentType() == Utils::SHADER_2D)
		ShaderManager::GetInstance().ApplyTexture("texture1");
	else if (ShaderManager::GetInstance().GetCurrentType() == Utils::SHADER_3D) {
		//SET THE SHADER TO DRAW BLENDING TEXTURE
		ShaderManager::GetInstance().ApplyUniformBool("isBlending", true);
		ShaderManager::GetInstance().ApplyTexture("objTexture");
	}
	
	//Draw tiles
	for (auto& tile : tiles) {
		if(drawingViewer)
			tile.second->Draw(tileWidth, tileHeight, 0, 0);
		else
			tile.second->Draw(tileWidth, tileHeight, (int)realPos.x, (int)realPos.y, (int)realPos.z);
	}

	//Disable the texture for tiles
	adr_glActiveTexture(GL_TEXTURE0);
	adr_glBindTexture(GL_TEXTURE_2D, 0);

	//Tile indicator
	if (tileIndicator.get() && drawTileIndicator) {
		//Set some stuff
		ShaderManager::GetInstance().ApplyUniformBool("drawTileIndicator", true);
		ShaderManager::GetInstance().ApplyUniformInt("tileWidth", tileWidth);
		ShaderManager::GetInstance().ApplyUniformInt("tileHeight", tileHeight);

		//Drawing
		tileIndicator->Draw(tileWidth, tileHeight, 0, 0);

		//Disabling
		ShaderManager::GetInstance().ApplyUniformBool("drawTileIndicator", false);
		drawTileIndicator = false;
	}
}

/*
PURPOSE: Add a tile which is selected from createdTiles to the map
*/
ENTITYMANAGER_API void TileMap::AddTileToMap(int mouseX, int mouseY, float cameraX, float cameraY, std::pair<int, int> selectedTile)
{
	//Get the selected tile and insert it to tiles to draw
	auto createdTileIter = createdTiles.find(selectedTile);
	if (createdTileIter == createdTiles.end())
		return;

	//Calculte position of the tile
	std::pair<int, int> tilePos = GetTilePos((float)mouseX, (float)mouseY, cameraX, cameraY);

	//Check if there is a tile on this coordinates
	auto tileIter = tiles.find({ tilePos.first, tilePos.second });

	//Add a tile if there is not
	if (tileIter == tiles.end()) {
		std::shared_ptr<Tile> tile = std::make_shared<Tile>(*createdTileIter->second.get());
		tiles.insert({ { tilePos.first, tilePos.second }, tile });
	}
}

/*
PURPOSE: Removes the tile which is located by mouse from the map
*/
ENTITYMANAGER_API void TileMap::RemoveTileFromMap(int mouseX, int mouseY, float cameraX, float cameraY)
{
	//Calculte position of the tile
	std::pair<int, int> tilePos = GetTilePos((float)mouseX, (float)mouseY, cameraX, cameraY);

	//Check if there is a tile on this coordinates
	auto tileIter = tiles.find({ tilePos.first, tilePos.second });

	//Remove the tile if there is
	if (tileIter != tiles.end()) {
		tiles.erase(tileIter);
	}
}

/*
PURPOSE: Draws a rectange to indicate which tile coordinates will be filled
*/
void TileMap::UpdateMouseTileIndicator(int mouseX, int mouseY, float cameraX, float cameraY)
{
	//Calculte position of the tile
	std::pair<int, int> tilePos = GetTilePos((float)mouseX, (float)mouseY, cameraX, cameraY);

	//Set position of the tile indicator and activate to show
	if (tileIndicator.get()) {
		tileIndicator->SetPos(tilePos.first, tilePos.second);
		drawTileIndicator = true;
	}
}

/*
PURPOSE: Calculates and returns the position of the tile that will be processed from camera and mouse positions
*/
ENTITYMANAGER_API std::pair<int, int> TileMap::GetTilePos(float mouseX, float mouseY, float cameraX, float cameraY)
{
	//Get the tile pos on the scene
	float tileXAtScene = mouseX + cameraX;
	float tileYAtScene = mouseY + cameraY;

	//Calculate the tile pos on the tileMap
	int tileX = (int)(tileXAtScene / (float)tileWidth);
	int tileY = (int)(tileYAtScene / (float)tileHeight);

	//We will add 1 when tile pos is sub-zero
	if (tileXAtScene < 0.0f)
		tileX--;
	if (tileYAtScene < 0.0f)
		tileY--;

	return { tileX, tileY };
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

	//Save tile size
	this->tileWidth = (int)tileWidth;
	this->tileHeight = (int)tileHeight;

	//Calculate tile count
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

			tile->Create(x, y, realTileWidth, realTileHeight, u, v, TTX, TTY, { x, y });

			createdTiles.insert({ { x, y }, std::shared_ptr<Tile>(tile) });
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

	for (auto& tile : tiles)
		tile.second->Release();
	tiles.clear();

	width *= scale;
	height *= scale;

	//------ RELEASING ------
	
	//Delete old Buffers
	if (inspectFramebuffer) {
		delete inspectFramebuffer;
		inspectFramebuffer = nullptr;
	}
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
	if (tileIndicator.get())
		tileIndicator->Release();

	//------ TILE POS INDICATOR ------

	tileIndicator = std::make_shared<Tile>();
	tileIndicator->Create(0, 0, tileW, tileH, 0.0f, 0.0f, 1.0f, 1.0f, { -1, -1 });

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
	adr_glGenBuffers(1, &VBO);
	adr_glGenBuffers(1, &EBO);

	adr_glGenVertexArrays(1, &VAO);
	adr_glBindVertexArray(VAO);

	adr_glBindBuffer(GL_ARRAY_BUFFER, VBO);
	adr_glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	adr_glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	adr_glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Set the vertex attrib pointers (position = 0)
	adr_glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	adr_glEnableVertexAttribArray(0);

	//Set the vertex attrib pointers (position = 1)
	adr_glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	adr_glEnableVertexAttribArray(1);

	//Release buffers
	adr_glBindBuffer(GL_ARRAY_BUFFER, 0);
	adr_glBindVertexArray(0);

	//------ FRAME BUFFER ------

	inspectFramebuffer = new FramebufferProvider();
	inspectFramebuffer->CreateFramebuffer((int)width, (int)height);
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
	inspectFramebuffer->BindFramebuffer();
	Graphics::GetInstance().Clear();
	adr_glViewport(0, 0, (GLsizei)(width * scale), (GLsizei)(height * scale));

	//Some calculations for transformation
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(scale, scale, 1.0f));
	glm::mat4 proj = glm::ortho(0.0f, (float)width * scale, (float)height * scale, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);

	//Send the transformation matrix to the shader
	ShaderManager::GetInstance().ApplyUniformMatrix("uModel", model);
	ShaderManager::GetInstance().ApplyUniformMatrix("uProjection", proj);
	ShaderManager::GetInstance().ApplyUniformMatrix("uView", view);
	ShaderManager::GetInstance().ApplyUniformInt("tileW", tileW);
	ShaderManager::GetInstance().ApplyUniformInt("tileH", tileH);
	ShaderManager::GetInstance().ApplyUniformInt("textureWidth", width);
	ShaderManager::GetInstance().ApplyUniformInt("textureHeight", height);

	//Set the texture
	adr_glActiveTexture(GL_TEXTURE0);
	adr_glBindTexture(GL_TEXTURE_2D, params->texture);
	ShaderManager::GetInstance().ApplyTexture("texture1");

	//Draw the texture
	adr_glBindVertexArray(VAO);
	adr_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	//Disable after drawing
	inspectFramebuffer->UnbindFramebuffer();
	adr_glActiveTexture(GL_TEXTURE0);
	adr_glBindTexture(GL_TEXTURE_2D, 0);
	adr_glBindVertexArray(0);

	ShaderManager::GetInstance().UseShaders(prevShaderType);
}

/*
PURPOSE: Check if buffers for inspector are created
*/
ENTITYMANAGER_API bool TileMap::IsInspectCreated()
{
	if (!inspectFramebuffer)
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
	return inspectFramebuffer->GetFrameBufferTex();
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
			//Get type
			int typeX = t.value("type-x", -1);
			int typeY = t.value("type-y", -1);

			//Create a new type of tile
			Tile* tile = new Tile();
			tile->Create(
				t.value("x", 0),
				t.value("y", 0),
				t.value("width", 0),
				t.value("height", 0),
				t.value("u", 0.0f),
				t.value("v", 0.0f),
				t.value("tex-w", 0.0f),
				t.value("tex-h", 0.0f),
				{ typeX, typeY }
			); // We don't have to have a "fromjson" function, "create" handles it

			//Add a new type
			createdTiles.insert({ { typeX, typeY }, std::shared_ptr<Tile>(tile) });
		}
	}

	//Tiles to draw
	if (json.contains("tiles")) {
		nlohmann::json tileJson = json["tiles"];
		for (auto& t : tileJson) {
			//Get position
			int x = t.value("x", 0);
			int y = t.value("y", 0);

			//Get the type of the tile
			int typeX = t.value("type-x", -1);
			int typeY = t.value("type-y", -1);

			//Check if this type of tile exists
			auto createdTileIter = createdTiles.find({ typeX, typeY });
			if (createdTileIter == createdTiles.end())
				return;

			//Check if there is a tile on this coordinates
			auto tileIter = tiles.find({ x, y });

			//Add a tile if there is not
			if (tileIter == tiles.end()) {
				std::shared_ptr<Tile> tile = std::make_shared<Tile>(*createdTileIter->second.get());
				tiles.insert({ { x, y }, tile });
			}
		}
	}

	// Tile size
	tileWidth = json.value("tile-width", 0);
	tileHeight = json.value("tile-height", 0);

	//Tile indicator
	tileIndicator = std::make_shared<Tile>();
	tileIndicator->Create(0, 0, tileWidth, tileHeight, 0.0f, 0.0f, 1.0f, 1.0f, { -1, -1 });
}
