#include "pch.h"
#include "FlipBook.h"

/*
PURPOSE: Initializes the entity
*/
ENTITYMANAGER_API bool FlipBook::CreateEntity(std::shared_ptr<EntityParams> params)
{
    //Cast EntityParams to FlipBookParams to use its properties
    auto casted = std::dynamic_pointer_cast<FlipBookParams>(params);
    if (!casted) {
        Logger::Log("E", "Casting failed at dynamic_cast<FlipBookParams*>(params)");
        return false;
    }
    this->params = casted;

    return true;
}

/*
PURPOSE: Unitializes the entity
*/
ENTITYMANAGER_API void FlipBook::DeleteEntity()
{
	for (auto& frame : createdFrames)
		frame.second->Release();
	createdFrames.clear();

	frames.clear();
}

/*
PURPOSE: Updates the entity
*/
ENTITYMANAGER_API void FlipBook::Update()
{
	if (!frames.empty()) {
		//Get elapsed time
		float currentTime = Timer::GetCurTime();

		float deltaTime = currentTime - lastTime;

		//Update current index
		if (deltaTime > frameWait && currentIndex != frames.size()) {
			currentIndex++;

			//Current index will continue from beginning of frames
			if (currentIndex == frames.size()) {
				if (loop)
					currentIndex = 0;
				else
					ended = true;
			}

			//Set the last time
			lastTime = currentTime;

			//Update current frame
			frames[currentIndex]->Update();
		}
	}
}

/*
PURPOSE: Draws the entity.
    This entity draws a sprite from a sprite array which is being displayed
*/
ENTITYMANAGER_API void FlipBook::Draw(glm::vec3 currentSceneCameraPos)
{
	if (!frames.empty()) {
		//Set the texture
		adr_glActiveTexture(GL_TEXTURE0);
		adr_glBindTexture(GL_TEXTURE_2D, params->texture);
		ShaderManager::GetInstance().ApplyTexture("texture1");

		//Draw the current frame
		frames[currentIndex]->Draw(frameWidth, frameHeight, realPos, realRot, realSca);

		//Disable the texture for tiles
		adr_glActiveTexture(GL_TEXTURE0);
		adr_glBindTexture(GL_TEXTURE_2D, 0);
	}
}

/*
PURPOSE: Creates each frame and starts updating it
*/
ENTITYMANAGER_API void FlipBook::CreateFrames(float textureWidth, float textureHeight, float frameWidth, float frameHeight)
{
	//Clear previous
	for (auto& frame : createdFrames)
		frame.second->Release();
	createdFrames.clear();
	frames.clear();

	//Save framesize
	this->frameWidth = (int)frameWidth;
	this->frameHeight = (int)frameHeight;

	//Calculate frame count
	int frameCountX = (int)std::ceil(textureWidth / frameWidth);
	int frameCountY = (int)std::ceil(textureHeight / frameHeight);

	for (int y = 0; y < frameCountY; ++y) {
		/*
			These calculations are for checking if some of frames don't have decided size.
			It they don't we will give them as most size as possible,
			In this scenario, we will give them the last texture size.
		*/
		//pass frame height into a new variable
		int realTileHeight = (int)frameHeight;

		//if heights of the frames aren't equal to each other
		if ((int)textureHeight % (int)frameHeight != 0) {
			//if this frame is the last in y coord in the flipbook
			if (y == frameCountY - 1) {
				//calculate y position of the frame in the flipbook and sub it from flipbook width
				int currentPosYInTileMap = y * (int)frameHeight;
				int lastTextureSizeY = (int)textureHeight - currentPosYInTileMap;

				//pass the new width
				realTileHeight = lastTextureSizeY;
			}
		}

		for (int x = 0; x < frameCountX; ++x) {
			//pass frame width into a new variable
			int realTileWidth = (int)frameWidth;

			//if widths of the frames aren't equal to each other
			if ((int)textureWidth % (int)frameWidth != 0) {

				//if this frame is the last in x coord in the flipbook
				if (x == frameCountX - 1) {
					//calculate x position of the tile in the flipbook and sub it from flipbook width
					int currentPosXInTileMap = x * (int)frameWidth;
					int lastTextureSizeX = (int)textureWidth - currentPosXInTileMap;

					//pass the new width
					realTileWidth = lastTextureSizeX;
				}
			}

			//Create frame and insert it into createdFrames
			FlipBookFrame* frame = new FlipBookFrame();
			float texturePosX = x * frameWidth;
			float texturePosY = y * frameHeight;

			/*
				Here we are translating pixel coordinates to[0.0, 1.0].
				For example, the texture width is 512 pixels, and the current tile pos is 256,
				After translation, the new x value will be 0.5
			*/
			float u = texturePosX * 1.0f / textureWidth;
			float v = texturePosY * 1.0f / textureHeight;

			//TF* stands for "translated frame width/height" btw
			float TFX = realTileWidth * 1.0f / textureWidth;
			float TFY = realTileHeight * 1.0f / textureHeight;

			frame->Create(x, y, realTileWidth, realTileHeight, u, v, TFX, TFY);

			createdFrames.push_back({ true, std::shared_ptr<FlipBookFrame>(frame) });
		}
	}
}

/*
PURPOSE: Creates a framebuffer and other buffer objects for the inspector
*/
ENTITYMANAGER_API void FlipBook::CreateInspectFrameBuffer(float textureWidth, float textureHeight, int frameWidth, int frameHeight)
{
    //------ RELEASING ------
	for (auto& frame : createdFrames)
		frame.second->Release();
	createdFrames.clear();
	frames.clear();
	
	//Delete old Buffers
	if (inspectFrameBuffer != -1) {
		adr_glDeleteFramebuffers(1, &inspectFrameBuffer);
		inspectFrameBuffer = -1;
	}
	if (inspectRenderBuffer != -1) {
		adr_glDeleteRenderbuffers(1, &inspectRenderBuffer);
		inspectRenderBuffer = -1;
	}
	if (inspectTexture != -1) {
		adr_glDeleteTextures(1, &inspectTexture);
		inspectTexture = -1;
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

	//------ TEXTURE FLIPBOOK ------

	//Vertices for the texture
	float vertices[] = {
		//X					Y		   Z	 U     V
		 textureWidth, textureHeight, 0.0f, 1.0f, 0.0f, // top right
		 0.0f,	       textureHeight, 0.0f, 0.0f, 0.0f, // bottom right
		 0.0f,         0.0f,          0.0f, 0.0f, 1.0f, // bottom left
		 textureWidth, 0.0f,          0.0f, 1.0f, 1.0f, // top left 
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

	//frame buffer
	adr_glGenFramebuffers(1, &inspectFrameBuffer);
	adr_glBindFramebuffer(GL_FRAMEBUFFER, inspectFrameBuffer);

	//generate a texture and bind it to frame buffer
	adr_glGenTextures(1, &inspectTexture);
	adr_glBindTexture(GL_TEXTURE_2D, inspectTexture);
	adr_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (GLsizei)textureWidth, (GLsizei)textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	adr_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	adr_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	adr_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, inspectTexture, 0);

	//check the status of frame buffer
	if (adr_glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		Logger::Log("E", "Framebuffer is not complete");

	//generate render buffer and bind it to the frame buffer
	adr_glGenRenderbuffers(1, &inspectRenderBuffer);
	adr_glBindRenderbuffer(GL_RENDERBUFFER, inspectRenderBuffer);
	adr_glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, (GLsizei)textureWidth, (GLsizei)textureHeight);
	adr_glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, inspectRenderBuffer);

	//release all buffers
	adr_glBindFramebuffer(GL_FRAMEBUFFER, 0);
	adr_glBindTexture(GL_TEXTURE_2D, 0);
	adr_glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

/*
PURPOSE: Check if buffers for inspector are created
*/
ENTITYMANAGER_API bool FlipBook::IsInspectCreated()
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
	return true;
}

/*
PURPOSE: Updates the inspector, its empty for now
*/
ENTITYMANAGER_API void FlipBook::UpdateInspect()
{
}

/*
PURPOSE: Renders the inspector to the framebuffer
*/
ENTITYMANAGER_API void FlipBook::DrawInspect(int width, int height, int frameW, int frameH)
{
	//Save the previous type of shader
	Utils::ShaderType prevShaderType = ShaderManager::GetInstance().GetCurrentType();

	//Change the shader to draw inspector
	ShaderManager::GetInstance().UseShaders(Utils::SHADER_INSPECT_TILE);

	//Set the framebuffer
	adr_glBindFramebuffer(GL_FRAMEBUFFER, inspectFrameBuffer);
	Graphics::GetInstance().Clear();
	adr_glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));

	//Some calculations for transformation
	glm::mat4 model = glm::mat4(1.0f);
	//model = glm::scale(model, glm::vec3(scale, scale, 1.0f));
	glm::mat4 proj = glm::ortho(0.0f, (float)width, (float)height, 0.0f);
	glm::mat4 view = glm::mat4(1.0f);

	//Send the transformation matrix to the shader
	ShaderManager::GetInstance().ApplyTransformMatrix("uModel", model);
	ShaderManager::GetInstance().ApplyTransformMatrix("uProjection", proj);
	ShaderManager::GetInstance().ApplyTransformMatrix("uView", view);
	ShaderManager::GetInstance().ApplyUniformInt("tileW", frameW);
	ShaderManager::GetInstance().ApplyUniformInt("tileH", frameH);
	ShaderManager::GetInstance().ApplyUniformInt("textureWidth", width);
	ShaderManager::GetInstance().ApplyUniformInt("textureHeight", height);

	//Set the texture
	adr_glActiveTexture(GL_TEXTURE0);
	adr_glBindTexture(GL_TEXTURE_2D, params->texture);
	ShaderManager::GetInstance().ApplyTexture("texture1");

	//Draw the texture
	adr_glBindVertexArray(VAO);
	adr_glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	adr_glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Disable after drawing
	adr_glActiveTexture(GL_TEXTURE0);
	adr_glBindTexture(GL_TEXTURE_2D, 0);
	adr_glBindVertexArray(0);

	ShaderManager::GetInstance().UseShaders(prevShaderType);
}

/*
PURPOSE: Gets the texture of the inspector
*/
ENTITYMANAGER_API unsigned int FlipBook::GetInspectTexture()
{
	return inspectTexture;
}

/*
PURPOSE: Returns created frames
*/
ENTITYMANAGER_API std::vector<std::pair<bool, std::shared_ptr<FlipBookFrame>>>& FlipBook::GetCreatedFrames()
{
	return createdFrames;
}

/*
PURPOSE: Returns frames
*/
ENTITYMANAGER_API std::vector<std::shared_ptr<FlipBookFrame>>& FlipBook::GetFrames()
{
	return frames;
}

/*
PURPOSE: Returns current frame as a pure pointer
*/
ENTITYMANAGER_API FlipBookFrame* FlipBook::GetCurrentFrame()
{
	return frames[currentIndex].get();
}

/*
PURPOSE: Creates buffers, a new "frames" vector for storing each frame that is selected and starts updating it
*/
ENTITYMANAGER_API void FlipBook::StartFlipBook()
{
	//Store selected frames into another vector
	for (auto& frame : createdFrames) {
		if (!frame.first)
			continue;

		frames.push_back(frame.second);
	}

	//Set current index at 0
	currentIndex = 0;
}

/*
PURPOSE: Returns properties of the entity as a pure pointer
*/
ENTITYMANAGER_API EntityParams* FlipBook::GetEntityParams()
{
    return params.get();
}

/*
PURPOSE: Allows us to create json content of properties which belongs to type of the entity and return it.
    "Entity" creates entity json, "Object" creates object json.
*/
ENTITYMANAGER_API nlohmann::json FlipBook::ToJson()
{
	nlohmann::json j; //For the entity

	if (params) {
		j = params->ToJson();
	}

	nlohmann::json c; //For created frames
	for (auto& frame : createdFrames) {
		//Add a selected status
		nlohmann::json frameJson = frame.second->ToJson();
		frameJson.push_back({ "selected", frame.first });
		c.push_back(frameJson);
	}
	j["created-frames"] = c;

	//Tile size
	j["frame-width"] = frameWidth;
	j["frame-height"] = frameHeight;

	return j;
}

/*
PURPOSE: FlipBook has a fromjson function to load existing drawing frames and created frames from json
*/
ENTITYMANAGER_API void FlipBook::FromJson(nlohmann::json json)
{
	//Tiles to brush
	if (json.contains("created-frames")) {
		nlohmann::json c = json["created-frames"];
		for (auto& t : c) {
			//Create a new type of tile
			bool selected = t.value("selected", true);

			FlipBookFrame* frame = new FlipBookFrame();
			frame->Create(
				t.value("x", 0),
				t.value("y", 0),
				t.value("width", 0),
				t.value("height", 0),
				t.value("u", 0.0f),
				t.value("v", 0.0f),
				t.value("tex-w", 0.0f),
				t.value("tex-h", 0.0f)
			); // We don't have to have a "fromjson" function, "create" handles it

			//Add a new type
			createdFrames.push_back({ selected, std::shared_ptr<FlipBookFrame>(frame) });
		}
	}

	StartFlipBook();
}
