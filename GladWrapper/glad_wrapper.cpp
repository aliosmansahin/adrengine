#include "pch.h"

#define BUILDING_ENGINE_DLL
#include "glad_wrapper.h"


namespace adr {

    ENGINE_GLAD_API PFNGLCLEARPROC adr_glClear = nullptr;
    ENGINE_GLAD_API PFNGLCLEARCOLORPROC adr_glClearColor = nullptr;
    ENGINE_GLAD_API PFNGLGENFRAMEBUFFERSPROC adr_glGenFramebuffers = nullptr;
    ENGINE_GLAD_API PFNGLBINDFRAMEBUFFERPROC adr_glBindFramebuffer = nullptr;
    ENGINE_GLAD_API PFNGLGENBUFFERSPROC adr_glGenBuffers = nullptr;
    ENGINE_GLAD_API PFNGLBINDBUFFERPROC adr_glBindBuffer = nullptr;
    ENGINE_GLAD_API PFNGLBUFFERDATAPROC adr_glBufferData = nullptr;
    ENGINE_GLAD_API PFNGLENABLEVERTEXATTRIBARRAYPROC adr_glEnableVertexAttribArray = nullptr;
    ENGINE_GLAD_API PFNGLVERTEXATTRIBPOINTERPROC adr_glVertexAttribPointer = nullptr;
    ENGINE_GLAD_API PFNGLGENVERTEXARRAYSPROC adr_glGenVertexArrays = nullptr;
    ENGINE_GLAD_API PFNGLBINDVERTEXARRAYPROC adr_glBindVertexArray = nullptr;
    ENGINE_GLAD_API PFNGLDELETEFRAMEBUFFERSPROC adr_glDeleteFramebuffers = nullptr;
    ENGINE_GLAD_API PFNGLDELETERENDERBUFFERSPROC adr_glDeleteRenderbuffers = nullptr;
    ENGINE_GLAD_API PFNGLDELETETEXTURESPROC adr_glDeleteTextures = nullptr;
    ENGINE_GLAD_API PFNGLTEXIMAGE2DPROC adr_glTexImage2D = nullptr;
    ENGINE_GLAD_API PFNGLTEXPARAMETERIPROC adr_glTexParameteri = nullptr;
    ENGINE_GLAD_API PFNGLGENERATEMIPMAPPROC adr_glGenerateMipmap = nullptr;
    ENGINE_GLAD_API PFNGLCHECKFRAMEBUFFERSTATUSPROC adr_glCheckFramebufferStatus = nullptr;
    ENGINE_GLAD_API PFNGLRENDERBUFFERSTORAGEPROC adr_glRenderbufferStorage = nullptr;
    ENGINE_GLAD_API PFNGLFRAMEBUFFERTEXTURE2DPROC adr_glFramebufferTexture2D = nullptr;
    ENGINE_GLAD_API PFNGLFRAMEBUFFERRENDERBUFFERPROC adr_glFramebufferRenderbuffer = nullptr;
    ENGINE_GLAD_API PFNGLGENTEXTURESPROC adr_glGenTextures = nullptr;
    ENGINE_GLAD_API PFNGLBINDTEXTUREPROC adr_glBindTexture = nullptr;
    ENGINE_GLAD_API PFNGLGENRENDERBUFFERSPROC adr_glGenRenderbuffers = nullptr;
    ENGINE_GLAD_API PFNGLBINDRENDERBUFFERPROC adr_glBindRenderbuffer = nullptr;
    ENGINE_GLAD_API PFNGLDELETEBUFFERSPROC adr_glDeleteBuffers = nullptr;
    ENGINE_GLAD_API PFNGLDELETEVERTEXARRAYSPROC adr_glDeleteVertexArrays = nullptr;
    ENGINE_GLAD_API PFNGLVIEWPORTPROC adr_glViewport = nullptr;
    ENGINE_GLAD_API PFNGLCREATESHADERPROC adr_glCreateShader = nullptr;
    ENGINE_GLAD_API PFNGLSHADERSOURCEPROC adr_glShaderSource = nullptr;
    ENGINE_GLAD_API PFNGLCOMPILESHADERPROC adr_glCompileShader = nullptr;
    ENGINE_GLAD_API PFNGLGETSHADERIVPROC adr_glGetShaderiv = nullptr;
    ENGINE_GLAD_API PFNGLGETSHADERINFOLOGPROC adr_glGetShaderInfoLog = nullptr;
    ENGINE_GLAD_API PFNGLDELETESHADERPROC adr_glDeleteShader = nullptr;
    ENGINE_GLAD_API PFNGLCREATEPROGRAMPROC adr_glCreateProgram = nullptr;
    ENGINE_GLAD_API PFNGLATTACHSHADERPROC adr_glAttachShader = nullptr;
    ENGINE_GLAD_API PFNGLLINKPROGRAMPROC adr_glLinkProgram = nullptr;
    ENGINE_GLAD_API PFNGLGETPROGRAMIVPROC adr_glGetProgramiv = nullptr;
    ENGINE_GLAD_API PFNGLGETPROGRAMINFOLOGPROC adr_glGetProgramInfoLog = nullptr;
    ENGINE_GLAD_API PFNGLUSEPROGRAMPROC adr_glUseProgram = nullptr;
    ENGINE_GLAD_API PFNGLGETUNIFORMLOCATIONPROC adr_glGetUniformLocation = nullptr;
    ENGINE_GLAD_API PFNGLUNIFORMMATRIX4FVPROC adr_glUniformMatrix4fv = nullptr;
    ENGINE_GLAD_API PFNGLUNIFORM3FPROC adr_glUniform3f = nullptr;
    ENGINE_GLAD_API PFNGLUNIFORM1IPROC adr_glUniform1i = nullptr;
    ENGINE_GLAD_API PFNGLUNIFORM1FPROC adr_glUniform1f = nullptr;
    ENGINE_GLAD_API PFNGLDELETEPROGRAMPROC adr_glDeleteProgram = nullptr;

    ENGINE_GLAD_API PFNGLENABLEPROC adr_glEnable = nullptr;
    ENGINE_GLAD_API PFNGLBLENDFUNCPROC adr_glBlendFunc = nullptr;
    ENGINE_GLAD_API PFNGLCULLFACEPROC adr_glCullFace = nullptr;
    ENGINE_GLAD_API PFNGLACTIVETEXTUREPROC adr_glActiveTexture = nullptr;
    ENGINE_GLAD_API PFNGLDISABLEPROC adr_glDisable = nullptr;
    ENGINE_GLAD_API PFNGLDRAWELEMENTSPROC adr_glDrawElements = nullptr;
    ENGINE_GLAD_API PFNGLDRAWARRAYSPROC adr_glDrawArrays = nullptr;

    ENGINE_GLAD_API PFNGLTEXPARAMETERFVPROC adr_glTexParameterfv = nullptr;
    ENGINE_GLAD_API PFNGLDRAWBUFFERPROC adr_glDrawBuffer = nullptr;
    ENGINE_GLAD_API PFNGLREADBUFFERPROC adr_glReadBuffer = nullptr;

    ENGINE_GLAD_API PFNGLFRAMEBUFFERTEXTUREPROC adr_glFramebufferTexture = nullptr;
}
