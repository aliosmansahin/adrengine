#include "pch.h"
#include "Graphics.h"

#include "ShaderManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stbi/stb_image.h"

/*
PURPOSE: Initialize graphics engine
*/
bool Graphics::InitGraphics(GLFWwindow* window)
{
	//logger
    Logger::Log("P", "Initalizing graphics");

	//set the glfw window
    this->window = window;

    //load opengl functions
    // 
    //int version = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    int version = gladLoadGL();
    if (version == 0) {
        Logger::Log("E", "Starting engine failed in gladLoadGL");
        return false;
    }

    Logger::Log("P", "Loaded OpenGL Version 3.3");

    //initilize shader manager
    Logger::Log("P", "Initializing shader manager");

    if (!ShaderManager::GetInstance().InitShaderManager())
        return false;

	//initialize each shader
    if (!ShaderManager::GetInstance().InitShaders(SHADER_2D))
        return false;
    if (!ShaderManager::GetInstance().InitShaders(SHADER_3D))
        return false;
    if (!ShaderManager::GetInstance().InitShaders(DEPTH))
        return false;
    if (!ShaderManager::GetInstance().InitShaders(DEPTH_CUBE))
        return false;

    return true;
}

/*
PURPOSE: Release graphics engine
*/
void Graphics::ReleaseGraphics()
{
	//Release shader manager
    ShaderManager::GetInstance().ReleaseShaderManager();
	//Release main framebuffer, renderbuffer, framebuffertexture
	glDeleteFramebuffers(1, &FBO);
	glDeleteRenderbuffers(1, &RBO);
	glDeleteTextures(1, &frameBufferTex);
	//logger
    Logger::Log("P", "Cleared graphics");
}

/*
PURPOSE: Create a main frame buffer to draw it on the imgui window as a texture
*/
void Graphics::CreateFramebuffer(int width, int height)
{
	//frame buffer
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	//generate a texture and bind it to frame buffer
    glGenTextures(1, &frameBufferTex);
    glBindTexture(GL_TEXTURE_2D, frameBufferTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTex, 0);

	//check the status of frame buffer
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Logger::Log("E", "Framebuffer is not complete");

	//generate render buffer and bind it to the frame buffer
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
    
	//release all buffers
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

/*
PURPOSE: Rescale frame buffer to handle changing gameviewport size
*/
void Graphics::RescaleFramebuffer(int width, int height)
{
    glBindTexture(GL_TEXTURE_2D, frameBufferTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTex, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}

/*
PURPOSE: Loads a texture from path and returns it
*/
unsigned int Graphics::LoadTexture(const char* id, const char* path, int& width, int& height)
{
	//generate a texture and bind it
    unsigned int texture;
	glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    //texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //load the image
    int nrChannels;
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data) {
        //check if it has alpha channel
        GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;

        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

		//generates mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
		//handle loading error
        std::string str = "Could not load texture \"";
        str += path;
        str += "\"";

        Logger::Log("E", str.c_str());
        return -1;
    }

	//release image data
    stbi_image_free(data);

	//message for logger
    std::string str = "Loaded texture \"";
    str += path;
    str += "\"";

    Logger::Log("P", str.c_str());

	//returns the generated texture
    return texture;
}

/*
PURPOSE: Release texture
*/
GRAPHICS_API void Graphics::UnloadTexture(unsigned int texture)
{
	glDeleteTextures(1, &texture);
}

/*
PURPOSE: Release mesh
*/
GRAPHICS_API void Graphics::UnloadMesh(unsigned int vao, unsigned int vboPos, unsigned int vboNormal, unsigned int vboUv)
{
	glDeleteBuffers(1, &vboPos);
	glDeleteBuffers(1, &vboNormal);
	glDeleteBuffers(1, &vboUv);
	glDeleteVertexArrays(1, &vao);
}

/*
PURPOSE: Loads texture from path and returns VAO, vertice count will be passed as a reference of parameter
*/
GRAPHICS_API unsigned int Graphics::LoadMesh(const char* id, const char* path, int& verticeCount)
{
	//some vectors
	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> uvIndices;
	std::vector<unsigned int> normalIndices;

	std::vector<glm::vec3> tmp_vertices;
	std::vector<glm::vec2> tmp_uvs;
	std::vector<glm::vec3> tmp_normals;

	//open the obj file
	std::ifstream objFile(path);
	if (!objFile.is_open()) {
		std::string str = "OBJ file \"";
		str += path;
		str += "\" was not found";

		Logger::Log("E", str.c_str());
		return false;
	}

	//read data and pass them to the vectors as the headers
	std::string header;
	while (objFile >> header) {
		if (header == "v") {
			glm::vec3 vertex;
			objFile >> vertex.x >> vertex.y >> vertex.z;
			tmp_vertices.push_back(vertex);
		}
		else if (header == "vt") {
			glm::vec2 uv;
			objFile >> uv.x >> uv.y;
			tmp_uvs.push_back(uv);
		}
		else if (header == "vn") {
			glm::vec3 normal;
			objFile >> normal.x >> normal.y >> normal.z;
			tmp_normals.push_back(normal);
		}
		else if (header == "f") {
			std::string vertex[3];
			objFile >> vertex[0] >> vertex[1] >> vertex[2];
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			for (int i = 0; i < 3; ++i) {
				int result = sscanf_s(vertex[i].c_str(), "%d/%d/%d", &vertexIndex[i], &uvIndex[i], &normalIndex[i]);
				if (result != 3)
					continue;
			}

			vertexIndices.push_back(vertexIndex[0]);
			vertexIndices.push_back(vertexIndex[1]);
			vertexIndices.push_back(vertexIndex[2]);
			uvIndices.push_back(uvIndex[0]);
			uvIndices.push_back(uvIndex[1]);
			uvIndices.push_back(uvIndex[2]);
			normalIndices.push_back(normalIndex[0]);
			normalIndices.push_back(normalIndex[1]);
			normalIndices.push_back(normalIndex[2]);
		}
	}
	objFile.close();

	//out vectors
	std::vector < glm::vec3 > out_vertices;
	std::vector < glm::vec2 > out_uvs;
	std::vector < glm::vec3 > out_normals;

	//process all vectors
	for (unsigned int i = 0; i < vertexIndices.size(); i++) {
		unsigned int vertexIndex = vertexIndices[i];

		glm::vec3 vertex = tmp_vertices[vertexIndex - 1];

		out_vertices.push_back(vertex);
	}
	for (unsigned int i = 0; i < uvIndices.size(); i++) {
		unsigned int uvIndex = uvIndices[i];

		glm::vec2 uv = tmp_uvs[uvIndex - 1];

		out_uvs.push_back(uv);
	}
	for (unsigned int i = 0; i < normalIndices.size(); i++) {
		unsigned int normalIndex = normalIndices[i];

		glm::vec3 normal = tmp_normals[normalIndex - 1];

		out_normals.push_back(normal);
	}

	//set the vertices count
	verticeCount = (int)out_vertices.size();

	//VBOs and VAO
	unsigned int VBO_positions;
	unsigned int VBO_normals;
	unsigned int VBO_uvs;

	unsigned int VAO;

	//create buffers, bind them and load arrays into buffers
	glGenBuffers(1, &VBO_positions);
	glGenBuffers(1, &VBO_normals);
	glGenBuffers(1, &VBO_uvs);
	//glGenBuffers(1, &EBO);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_positions);
	glBufferData(GL_ARRAY_BUFFER, out_vertices.size() * sizeof(glm::vec3), &out_vertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
	glBufferData(GL_ARRAY_BUFFER, out_normals.size() * sizeof(glm::vec3), &out_normals[0], GL_STATIC_DRAW);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glEnableVertexAttribArray(1);
	/*glBindBuffer(GL_ARRAY_BUFFER, VBO_uv);

	/*glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);*/

	//set the vertex attrib pointers

	//set the vertex attrib pointers
	/*glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);*/

	//release buffers
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	std::string str = "Loaded mesh \"";
	str += path;
	str += "\"";

	Logger::Log("P", str.c_str());
	return VAO;
}

/*
PURPOSE: To get the instance
*/
Graphics& Graphics::GetInstance()
{
    static Graphics graphics;
    return graphics;
}

/*
PURPOSE: Handles resizing window and changes the viewport
*/
void Graphics::WindowSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

/*
PURPOSE: Clears the drawing buffer
*/
void Graphics::Clear()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0, 0, 0, 0);
}
