#include "pch.h"
#include "FramebufferProvider.h"

/*
PURPOSE: Triggers ReleaseFramebuffer method
*/
FramebufferProvider::~FramebufferProvider()
{
    ReleaseFramebuffer();
}

/*
PURPOSE: Creates a frame buffer, render buffer, and a texture for frame buffer
*/
GRAPHICS_API void FramebufferProvider::CreateFramebuffer(int width, int height)
{
    //frame buffer
    adr::adr_glGenFramebuffers(1, &FBO);
    adr::adr_glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    //generate a texture and bind it to frame buffer
    adr::adr_glGenTextures(1, &frameBufferTex);

    UpdateFrameBufferTexture(width, height);

    //check the status of frame buffer
    if (adr::adr_glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        Logger::Log("E", "Framebuffer is not complete");

    //generate render buffer and bind it to the frame buffer
    adr::adr_glGenRenderbuffers(1, &RBO);

    UpdateRenderBuffer(width, height);

    //release all buffers
    adr::adr_glBindFramebuffer(GL_FRAMEBUFFER, 0);
    adr::adr_glBindTexture(GL_TEXTURE_2D, 0);
    adr::adr_glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

/*
PURPOSE: Rescales frame buffer to handle changing gameviewport size
*/
GRAPHICS_API void FramebufferProvider::RescaleFramebuffer(int width, int height)
{
    //rescale texture
    UpdateFrameBufferTexture(width, height);

    //rescale render buffer
    UpdateRenderBuffer(width, height);
}

/*
PURPOSE: Releases buffers
*/
GRAPHICS_API void FramebufferProvider::ReleaseFramebuffer()
{
    adr::adr_glDeleteFramebuffers(1, &FBO);
    adr::adr_glDeleteRenderbuffers(1, &RBO);
    adr::adr_glDeleteTextures(1, &frameBufferTex);
}

/*
PURPOSE: Updates properties of the texture of the framebuffer
*/
void FramebufferProvider::UpdateFrameBufferTexture(int width, int height)
{
    adr::adr_glBindTexture(GL_TEXTURE_2D, frameBufferTex);
    adr::adr_glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    adr::adr_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    adr::adr_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    adr::adr_glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frameBufferTex, 0);
}

/*
PURPOSE: Updates properties of the renderbuffer of the framebuffer
*/
void FramebufferProvider::UpdateRenderBuffer(int width, int height)
{
    adr::adr_glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    adr::adr_glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    adr::adr_glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);
}
