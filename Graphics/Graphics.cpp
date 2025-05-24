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
GRAPHICS_API void Graphics::UnloadMesh(std::vector<std::shared_ptr<ObjectMtl>>& objects)
{
	for (auto& object : objects) {
		glDeleteTextures(1, &object->material.diffuseTexture);
		glDeleteBuffers(1, &object->VBO_positions);
		glDeleteBuffers(1, &object->VBO_normals);
		glDeleteBuffers(1, &object->VBO_uvs);
		glDeleteVertexArrays(1, &object->VAO);
	}
}

/*
PURPOSE: Loads materials from file and pass it into the materials map
*/
GRAPHICS_API std::unordered_map<std::string, Material> Graphics::LoadMaterial(const char* path)
{
	std::unordered_map<std::string, Material> materials;
	std::ifstream file(path);

	if (!file.is_open()) {
		std::string str = "MTL file \"";
		str += path;
		str += "\" was not found";
		Logger::Log("E", str.c_str());
		return materials; // boþ dönebiliriz
	}

	std::string line;
	Material current;

	while (std::getline(file, line)) {
		std::istringstream iss(line);
		std::string keyword;
		iss >> keyword;

		if (keyword == "newmtl") {
			if (!current.name.empty()) {
				materials[current.name] = current;
				current = Material();
			}
			iss >> current.name;
		}
		else if (keyword == "Ka") { // Ambient
			iss >> current.Ka.r >> current.Ka.g >> current.Ka.b;
		}
		else if (keyword == "Kd") { // Diffuse
			iss >> current.Kd.r >> current.Kd.g >> current.Kd.b;
		}
		else if (keyword == "Ks") { // Specular
			iss >> current.Ks.r >> current.Ks.g >> current.Ks.b;
		}
		else if (keyword == "Ns") { // Shininess
			iss >> current.Ns;
		}
		else if (keyword == "map_Kd") { // Diffuse Texture
			iss >> current.map_Kd;
			int texW, texH;
			current.diffuseTexture = LoadTexture(current.map_Kd.c_str(), current.map_Kd.c_str(), texW, texH);
		}
		else if (keyword == "Ke") {  // Emissive color
			iss >> current.Ke.r >> current.Ke.g >> current.Ke.b;
		}
		else if (keyword == "Ni") {  // Optical density (refractive index)
			iss >> current.Ni;
		}
		else if (keyword == "d") {   // Opacity
			iss >> current.d;
		}
		else if (keyword == "Tr") {  // Alternative opacity (transparent)
			iss >> current.d;
			current.d = 1.0f - current.d; // Tr is inverse of d
		}
		else if (keyword == "illum") { // Illumination model
			iss >> current.illum;
		}
	}

	if (!current.name.empty()) {
		materials[current.name] = current;
	}

	return materials;
}

/*
PURPOSE: Loads texture from path and returns VAO, vertice count will be passed as a reference of parameter
*/
GRAPHICS_API std::vector<std::shared_ptr<ObjectMtl>> Graphics::LoadMesh(const char* id, const char* path)
{
	//some vectors
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
		return std::vector<std::shared_ptr<ObjectMtl>>();
	}

	//Store materials
	std::unordered_map<std::string, Material> materials;
	std::vector<ObjectMtlNotIndexed> objectMtls;

	//Read data and pass them to the vectors as headers
	std::string header;
	ObjectMtlNotIndexed objectMtl;
	while (objFile >> header) {
		if (header == "mtllib") {//This is the material file of object
			objFile >> header; //Read the file path
			materials = LoadMaterial(header.c_str());

			if (materials.empty())
				Logger::Log("I", "No material found");
		}
		else if (header == "v") {
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
		else if (header == "usemtl") {
			//This is to know which material will be used
			if (!objectMtl.name.empty()) {
				objectMtls.push_back(objectMtl);
				objectMtl = ObjectMtlNotIndexed();
			}
			objFile >> header;
			objectMtl.name = header;
			objectMtl.material = materials[header];
		}
		else if (header == "f") {
			//Organize vertices
			std::string vertex[3];
			objFile >> vertex[0] >> vertex[1] >> vertex[2];
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
			bool hasVT = true;

			for (int i = 0; i < 3; ++i) {
				if (vertex[i].find("//") != std::string::npos) {
					// v//vn
					sscanf_s(vertex[i].c_str(), "%d//%d", &vertexIndex[i], &normalIndex[i]);
					uvIndex[i] = 0; // no vt
					hasVT = false;
				}
				else {
					// v/vt/vn
					sscanf_s(vertex[i].c_str(), "%d/%d/%d", &vertexIndex[i], &uvIndex[i], &normalIndex[i]);
				}

			}
			
			objectMtl.vertexIndices.push_back(vertexIndex[0]);
			objectMtl.vertexIndices.push_back(vertexIndex[1]);
			objectMtl.vertexIndices.push_back(vertexIndex[2]);
			if (hasVT) {
				objectMtl.uvIndices.push_back(uvIndex[0]);
				objectMtl.uvIndices.push_back(uvIndex[1]);
				objectMtl.uvIndices.push_back(uvIndex[2]);
			}
			objectMtl.normalIndices.push_back(normalIndex[0]);
			objectMtl.normalIndices.push_back(normalIndex[1]);
			objectMtl.normalIndices.push_back(normalIndex[2]);
		}
	}
	objFile.close();

	//Add last objectMtl into the vector
	if (!objectMtl.name.empty()) {
		objectMtls.push_back(objectMtl);
	}

	//out objectMtls
	std::vector<std::shared_ptr<ObjectMtl>> out_objectMtls;
	
	//Index each objectMtl
	for (int i = 0; i < objectMtls.size(); ++i) {
		auto& oldObjectMtl = objectMtls[i];
		std::shared_ptr<ObjectMtl> out_objectMtl = std::make_shared<ObjectMtl>();

		//process all vectors
		for (unsigned int i = 0; i < oldObjectMtl.vertexIndices.size(); i++) {
			unsigned int vertexIndex = oldObjectMtl.vertexIndices[i];

			glm::vec3 vertex = tmp_vertices[vertexIndex - 1];

			out_objectMtl->vertexIndices.push_back(vertex);
		}
		for (unsigned int i = 0; i < oldObjectMtl.uvIndices.size(); i++) {
			unsigned int uvIndex = oldObjectMtl.uvIndices[i];

			glm::vec2 uv = tmp_uvs[uvIndex - 1];

			out_objectMtl->uvIndices.push_back(uv);
		}
		for (unsigned int i = 0; i < oldObjectMtl.normalIndices.size(); i++) {
			unsigned int normalIndex = oldObjectMtl.normalIndices[i];

			glm::vec3 normal = tmp_normals[normalIndex - 1];

			out_objectMtl->normalIndices.push_back(normal);
		}

		//set the vertices count
		out_objectMtl->verticeCount = (int)out_objectMtl->vertexIndices.size();
		out_objectMtl->material = oldObjectMtl.material;

		//------- CREATE ALL BUFFERS OF EACH OBJECTMTL -------

		//create buffers, bind them and load arrays into buffers
		glGenBuffers(1, &out_objectMtl->VBO_positions);
		glGenBuffers(1, &out_objectMtl->VBO_normals);
		glGenBuffers(1, &out_objectMtl->VBO_uvs);
		//glGenBuffers(1, &EBO);

		glGenVertexArrays(1, &out_objectMtl->VAO);
		glBindVertexArray(out_objectMtl->VAO);

		glBindBuffer(GL_ARRAY_BUFFER, out_objectMtl->VBO_positions);
		glBufferData(GL_ARRAY_BUFFER, out_objectMtl->vertexIndices.size() * sizeof(glm::vec3), &out_objectMtl->vertexIndices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		//If there is a texture in the material, create uv buffer
		if (!out_objectMtl->uvIndices.empty()) {
			glBindBuffer(GL_ARRAY_BUFFER, out_objectMtl->VBO_uvs);
			glBufferData(GL_ARRAY_BUFFER, out_objectMtl->uvIndices.size() * sizeof(glm::vec2), &out_objectMtl->uvIndices[0], GL_STATIC_DRAW);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
			glEnableVertexAttribArray(1);
		}

		glBindBuffer(GL_ARRAY_BUFFER, out_objectMtl->VBO_normals);
		glBufferData(GL_ARRAY_BUFFER, out_objectMtl->normalIndices.size() * sizeof(glm::vec3), &out_objectMtl->normalIndices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(2);
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

		out_objectMtls.push_back(out_objectMtl);
	}

	std::string str = "Loaded mesh \"";
	str += path;
	str += "\"";

	Logger::Log("P", str.c_str());
	return out_objectMtls;
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
