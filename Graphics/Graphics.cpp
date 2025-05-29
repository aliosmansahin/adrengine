#include "pch.h"
#include "Graphics.h"
#include "Timer.h"

#include "ShaderManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stbi/stb_image.h"


#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

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
    if (!ShaderManager::GetInstance().InitShaders(Utils::SHADER_2D))
        return false;
    if (!ShaderManager::GetInstance().InitShaders(Utils::SHADER_3D))
        return false;
    if (!ShaderManager::GetInstance().InitShaders(Utils::DEPTH))
        return false;
    if (!ShaderManager::GetInstance().InitShaders(Utils::DEPTH_CUBE))
        return false;
	if (!ShaderManager::GetInstance().InitShaders(Utils::SHADER_INSPECT_TILE))
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
GRAPHICS_API bool Graphics::LoadMaterial(const char* path, std::unordered_map<std::string, Material>& materials)
{
	std::ifstream file(path);

	if (!file.is_open()) {
		std::string str = "MTL file \"";
		str += path;
		str += "\" was not found";
		Logger::Log("E", str.c_str());
		return false;
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

	return true;
}

/*
PURPOSE: Returns the directorty of the file path
*/
std::string ExtractDirectoryFromPath(const std::string& path) {
	size_t found = path.find_last_of("/\\");
	if (found != std::string::npos)
		return path.substr(0, found + 1);
	return "";
}

/*
PURPOSE: Creates a Material struct from TinyObj material_t object and returns it
*/
Material LoadMaterialFromTinyObj(int mat_id, const std::vector<tinyobj::material_t>& materials) {
	Material m;
	if (mat_id >= 0 && mat_id < materials.size()) {
		const auto& mat = materials[mat_id];
		m.name = mat.name;
		m.Ka = glm::vec3(mat.ambient[0], mat.ambient[1], mat.ambient[2]);
		m.Kd = glm::vec3(mat.diffuse[0], mat.diffuse[1], mat.diffuse[2]);
		m.Ks = glm::vec3(mat.specular[0], mat.specular[1], mat.specular[2]);
		m.Ke = glm::vec3(mat.emission[0], mat.emission[1], mat.emission[2]);
		m.Ns = mat.shininess;
		m.Ni = mat.ior;
		m.d = mat.dissolve;
		m.illum = mat.illum;
		m.map_Kd = mat.diffuse_texname;
		if (!m.map_Kd.empty()) {
			int w, h;
			m.diffuseTexture = Graphics::GetInstance().LoadTexture(m.map_Kd.c_str(), m.map_Kd.c_str(), w, h);
		}
	}
	else {
		m.name = "default";
	}
	return m;
}

/*
PURPOSE: Loads texture from path and returns VAO, vertice count will be passed as a reference of parameter
*/
GRAPHICS_API bool Graphics::LoadMesh(const char* id, const char* path, std::vector<std::shared_ptr<ObjectMtl>>& out_objectMtls)
{
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;

	Timer::StartChrono("meshLoad"); //START TIME //TODO:

	//Load obj file
	std::string base_dir = ExtractDirectoryFromPath(path); // helper: Get directiory from path
	bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path, base_dir.c_str());

	if (!warn.empty()) {
		Logger::Log("W", warn.c_str());
	}
	if (!err.empty()) {
		Logger::Log("E", err.c_str());
	}
	if (!ret) {
		return false;
	}

	// Parse each shape
	for (const auto& shape : shapes) {
		// Create ObjectMtl s for each material
		std::unordered_map<int, std::shared_ptr<ObjectMtl>> materialObjects;

		size_t index_offset = 0;
		for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
			int fv = shape.mesh.num_face_vertices[f]; // Usually 3

			//Get the material id
			int mat_id = -1;
			if (f < shape.mesh.material_ids.size()) {
				mat_id = shape.mesh.material_ids[f];
			}

			//Check if the material exists
			if (materialObjects.find(mat_id) == materialObjects.end()) {
				auto object = std::make_shared<ObjectMtl>();
				object->material = LoadMaterialFromTinyObj(mat_id, materials);
				materialObjects[mat_id] = object;
			}

			auto& object = materialObjects[mat_id];

			//Pass vertex, normal, uv
			for (size_t v = 0; v < fv; v++) {
				tinyobj::index_t idx = shape.mesh.indices[index_offset + v];

				glm::vec3 vertex = {
					attrib.vertices[3 * idx.vertex_index + 0],
					attrib.vertices[3 * idx.vertex_index + 1],
					attrib.vertices[3 * idx.vertex_index + 2]
				};
				object->vertexIndices.push_back(vertex);

				if (idx.normal_index >= 0) {
					glm::vec3 normal = {
						attrib.normals[3 * idx.normal_index + 0],
						attrib.normals[3 * idx.normal_index + 1],
						attrib.normals[3 * idx.normal_index + 2]
					};
					object->normalIndices.push_back(normal);
				}

				if (idx.texcoord_index >= 0) {
					glm::vec2 uv = {
						attrib.texcoords[2 * idx.texcoord_index + 0],
						attrib.texcoords[2 * idx.texcoord_index + 1]
					};
					object->uvIndices.push_back(uv);
				}
			}

			index_offset += fv;
		}

		//Create buffers for each materialObject
		for (const auto& pair : materialObjects) {
			auto& object = pair.second;
			object->verticeCount = static_cast<int>(object->vertexIndices.size());

			// VAO / VBO setup
			glGenBuffers(1, &object->VBO_positions);
			glGenBuffers(1, &object->VBO_normals);
			glGenBuffers(1, &object->VBO_uvs);
			glGenVertexArrays(1, &object->VAO);
			glBindVertexArray(object->VAO);

			if (!object->vertexIndices.empty()) {
				glBindBuffer(GL_ARRAY_BUFFER, object->VBO_positions);
				glBufferData(GL_ARRAY_BUFFER, object->vertexIndices.size() * sizeof(glm::vec3), &object->vertexIndices[0], GL_STATIC_DRAW);
				glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
				glEnableVertexAttribArray(0);
			}

			if (!object->uvIndices.empty()) {
				glBindBuffer(GL_ARRAY_BUFFER, object->VBO_uvs);
				glBufferData(GL_ARRAY_BUFFER, object->uvIndices.size() * sizeof(glm::vec2), &object->uvIndices[0], GL_STATIC_DRAW);
				glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
				glEnableVertexAttribArray(1);
			}

			if (!object->normalIndices.empty()) {
				glBindBuffer(GL_ARRAY_BUFFER, object->VBO_normals);
				glBufferData(GL_ARRAY_BUFFER, object->normalIndices.size() * sizeof(glm::vec3), &object->normalIndices[0], GL_STATIC_DRAW);
				glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
				glEnableVertexAttribArray(2);
			}

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);

			out_objectMtls.push_back(object);
		}
	}

	//Calculation for loading time
	float loadDuration = Timer::EndChrono("meshLoad");

	std::string log = std::string("Loaded mesh using TinyObjLoader: ") + path + " in " + std::to_string(loadDuration) + " seconds";
	Logger::Log("P", log.c_str());
	return true;
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
