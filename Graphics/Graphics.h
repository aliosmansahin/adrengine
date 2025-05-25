#pragma once

#ifdef GRAPHICS_EXPORTS
#define GRAPHICS_API __declspec(dllexport)
#else
#define GRAPHICS_API __declspec(dllimport)
#endif

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include <mutex>
#include <thread>
#include <glm/gtc/matrix_transform.hpp>

#include "Logger.h"

#include <chrono> // TODO: REMOVE IT AFTER OPTIMIZATION

//Store each material
struct Material {
	std::string name;
	glm::vec3 Ka; // Ambient
	glm::vec3 Kd; // Diffuse
	glm::vec3 Ks; // Specular
	float Ns = 0.0f; // Shininess
	glm::vec3 Ke; // Emissive
	float Ni = 1.0f; // Optical density (refractive index)
	float d = 1.0f; // Opacity
	int illum = 0; // Illumination model
	std::string map_Kd; // Diffuse texture path
	unsigned int diffuseTexture = -1;// Diffuse texture
};

//This is for every object that uses a material, also this struct will be indexed after loading
struct ObjectMtlNotIndexed {
	std::string name;
	std::vector<unsigned int> vertexIndices;
	std::vector<unsigned int> uvIndices;
	std::vector<unsigned int> normalIndices;

	Material material;
};

//This is for every object that uses a material, this is the final of loading
struct ObjectMtl {
	std::vector<glm::vec3> vertexIndices;
	std::vector<glm::vec2> uvIndices;
	std::vector<glm::vec3> normalIndices;

	Material material;
	int verticeCount;

	unsigned int VBO_positions;
	unsigned int VBO_normals;
	unsigned int VBO_uvs;

	unsigned int VAO;
};

class Graphics
{
public:
	//main funcs
	GRAPHICS_API bool			InitGraphics(GLFWwindow* window);
	GRAPHICS_API void			ReleaseGraphics();
	GRAPHICS_API void			CreateFramebuffer(int width, int height);
	GRAPHICS_API void			RescaleFramebuffer(int width, int height);
	GRAPHICS_API unsigned int	LoadTexture(const char* id, const char* path, int& width, int& height);
	GRAPHICS_API void			UnloadTexture(unsigned int texture);
	GRAPHICS_API void			UnloadMesh(std::vector<std::shared_ptr<ObjectMtl>>& objects);
	GRAPHICS_API bool			LoadMesh(const char* id, const char* path, std::vector<std::shared_ptr<ObjectMtl>>& out_objectMtls);
private:
	GRAPHICS_API bool			LoadMaterial(const char* path, std::unordered_map<std::string, Material>& materials);
public:
	//getter for the instance
	GRAPHICS_API static Graphics& GetInstance();
private:
	//singleton
	Graphics() = default;
	~Graphics() = default;
	Graphics(const Graphics&) = delete;
	Graphics& operator=(const Graphics&) = delete;
public:
	//callback funcs
	GRAPHICS_API void WindowSizeCallback(GLFWwindow* window, int width, int height);
	GRAPHICS_API static void StaticWindowSizeCallback(GLFWwindow* window, int width, int height) {
		Graphics* self = static_cast<Graphics*>(glfwGetWindowUserPointer(window));
		if (self) {
			self->WindowSizeCallback(window, width, height);
		}
	}
public:
	//context
	GRAPHICS_API void Clear();
	GRAPHICS_API void BindFramebuffer()   { glBindFramebuffer(GL_FRAMEBUFFER, FBO); }
	GRAPHICS_API void UnbindFramebuffer() { glBindFramebuffer(GL_FRAMEBUFFER, 0); }
public:
	//getters
	GRAPHICS_API GLFWwindow*  GetWindow()         { return window; }
	GRAPHICS_API unsigned int GetFrameBufferTex() { return frameBufferTex; }
private:
	//glfw
	GLFWwindow* window = nullptr;

	//framebuffer
	unsigned int FBO;
	unsigned int RBO;
	unsigned int frameBufferTex;
};