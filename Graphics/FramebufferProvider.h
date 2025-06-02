#pragma once

#ifdef GRAPHICS_EXPORTS
#define GRAPHICS_API __declspec(dllexport)
#else
#define GRAPHICS_API __declspec(dllimport)
#endif

#include "glad_wrapper.h"

#include "Logger.h"

class FramebufferProvider
{
public:
	//management
	GRAPHICS_API ~FramebufferProvider();
	GRAPHICS_API void CreateFramebuffer(int width, int height);
	GRAPHICS_API void RescaleFramebuffer(int width, int height);
	GRAPHICS_API void ReleaseFramebuffer();

	//binding
	GRAPHICS_API void BindFramebuffer() { adr::adr_glBindFramebuffer(GL_FRAMEBUFFER, FBO); }
	GRAPHICS_API void UnbindFramebuffer() { adr::adr_glBindFramebuffer(GL_FRAMEBUFFER, 0); }

	//Getter
	GRAPHICS_API unsigned int GetFrameBufferTex() { return frameBufferTex; }
private:
	//framebuffer
	unsigned int FBO;
	unsigned int RBO;
	unsigned int frameBufferTex;
};