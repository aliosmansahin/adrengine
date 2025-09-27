#pragma once
#include "glad/glad.h"

#ifdef BUILDING_ENGINE_DLL
#define ENGINE_GLAD_API __declspec(dllexport)
#else
#define ENGINE_GLAD_API __declspec(dllimport)
#endif

/*
    This is for sharing glad functions between all DLLs.
    Window class loads all functions, after that, adr_* variables will be set to functions.
    This is needed because loaded functions in Adrengine.exe cannot be used directly in other DLLs
    There is some pointer errors.
    In order to fix this problem, we are doing a some kind of sharing process.
    This function pointers can be accessed in other DLLs by including this file.
*/

namespace adr {
    extern ENGINE_GLAD_API PFNGLCLEARPROC adr_glClear;
    extern ENGINE_GLAD_API PFNGLCLEARCOLORPROC adr_glClearColor;
    extern ENGINE_GLAD_API PFNGLGENFRAMEBUFFERSPROC adr_glGenFramebuffers;
    extern ENGINE_GLAD_API PFNGLBINDFRAMEBUFFERPROC adr_glBindFramebuffer;
    extern ENGINE_GLAD_API PFNGLGENBUFFERSPROC adr_glGenBuffers;
    extern ENGINE_GLAD_API PFNGLBINDBUFFERPROC adr_glBindBuffer;
    extern ENGINE_GLAD_API PFNGLBUFFERDATAPROC adr_glBufferData;
    extern ENGINE_GLAD_API PFNGLENABLEVERTEXATTRIBARRAYPROC adr_glEnableVertexAttribArray;
    extern ENGINE_GLAD_API PFNGLVERTEXATTRIBPOINTERPROC adr_glVertexAttribPointer;
    extern ENGINE_GLAD_API PFNGLGENVERTEXARRAYSPROC adr_glGenVertexArrays;
    extern ENGINE_GLAD_API PFNGLBINDVERTEXARRAYPROC adr_glBindVertexArray;
    extern ENGINE_GLAD_API PFNGLDELETEFRAMEBUFFERSPROC adr_glDeleteFramebuffers;
    extern ENGINE_GLAD_API PFNGLDELETERENDERBUFFERSPROC adr_glDeleteRenderbuffers;
    extern ENGINE_GLAD_API PFNGLDELETETEXTURESPROC adr_glDeleteTextures;
    extern ENGINE_GLAD_API PFNGLTEXIMAGE2DPROC adr_glTexImage2D;
    extern ENGINE_GLAD_API PFNGLTEXPARAMETERIPROC adr_glTexParameteri;
    extern ENGINE_GLAD_API PFNGLGENERATEMIPMAPPROC adr_glGenerateMipmap;
    extern ENGINE_GLAD_API PFNGLCHECKFRAMEBUFFERSTATUSPROC adr_glCheckFramebufferStatus;
    extern ENGINE_GLAD_API PFNGLRENDERBUFFERSTORAGEPROC adr_glRenderbufferStorage;
    extern ENGINE_GLAD_API PFNGLFRAMEBUFFERTEXTURE2DPROC adr_glFramebufferTexture2D;
    extern ENGINE_GLAD_API PFNGLFRAMEBUFFERRENDERBUFFERPROC adr_glFramebufferRenderbuffer;
    extern ENGINE_GLAD_API PFNGLGENTEXTURESPROC adr_glGenTextures;
    extern ENGINE_GLAD_API PFNGLBINDTEXTUREPROC adr_glBindTexture;
    extern ENGINE_GLAD_API PFNGLGENRENDERBUFFERSPROC adr_glGenRenderbuffers;
    extern ENGINE_GLAD_API PFNGLBINDRENDERBUFFERPROC adr_glBindRenderbuffer;
    extern ENGINE_GLAD_API PFNGLDELETEBUFFERSPROC adr_glDeleteBuffers;
    extern ENGINE_GLAD_API PFNGLDELETEVERTEXARRAYSPROC adr_glDeleteVertexArrays;
    extern ENGINE_GLAD_API PFNGLVIEWPORTPROC adr_glViewport;
    extern ENGINE_GLAD_API PFNGLCREATESHADERPROC adr_glCreateShader;
    extern ENGINE_GLAD_API PFNGLSHADERSOURCEPROC adr_glShaderSource;
    extern ENGINE_GLAD_API PFNGLCOMPILESHADERPROC adr_glCompileShader;
    extern ENGINE_GLAD_API PFNGLGETSHADERIVPROC adr_glGetShaderiv;
    extern ENGINE_GLAD_API PFNGLGETSHADERINFOLOGPROC adr_glGetShaderInfoLog;
    extern ENGINE_GLAD_API PFNGLDELETESHADERPROC adr_glDeleteShader;
    extern ENGINE_GLAD_API PFNGLCREATEPROGRAMPROC adr_glCreateProgram;
    extern ENGINE_GLAD_API PFNGLATTACHSHADERPROC adr_glAttachShader;
    extern ENGINE_GLAD_API PFNGLLINKPROGRAMPROC adr_glLinkProgram;
    extern ENGINE_GLAD_API PFNGLGETPROGRAMIVPROC adr_glGetProgramiv;
    extern ENGINE_GLAD_API PFNGLGETPROGRAMINFOLOGPROC adr_glGetProgramInfoLog;
    extern ENGINE_GLAD_API PFNGLUSEPROGRAMPROC adr_glUseProgram;
    extern ENGINE_GLAD_API PFNGLGETUNIFORMLOCATIONPROC adr_glGetUniformLocation;
    extern ENGINE_GLAD_API PFNGLUNIFORMMATRIX4FVPROC adr_glUniformMatrix4fv;
    extern ENGINE_GLAD_API PFNGLUNIFORM3FPROC adr_glUniform3f;
    extern ENGINE_GLAD_API PFNGLUNIFORM1IPROC adr_glUniform1i;
    extern ENGINE_GLAD_API PFNGLUNIFORM1FPROC adr_glUniform1f;
    extern ENGINE_GLAD_API PFNGLDELETEPROGRAMPROC adr_glDeleteProgram;
    extern ENGINE_GLAD_API PFNGLENABLEPROC adr_glEnable;
    extern ENGINE_GLAD_API PFNGLBLENDFUNCPROC adr_glBlendFunc;
    extern ENGINE_GLAD_API PFNGLCULLFACEPROC adr_glCullFace;
    extern ENGINE_GLAD_API PFNGLACTIVETEXTUREPROC adr_glActiveTexture;
    extern ENGINE_GLAD_API PFNGLDISABLEPROC adr_glDisable;
    extern ENGINE_GLAD_API PFNGLDRAWELEMENTSPROC adr_glDrawElements;
    extern ENGINE_GLAD_API PFNGLDRAWARRAYSPROC adr_glDrawArrays;
    extern ENGINE_GLAD_API PFNGLTEXPARAMETERFVPROC adr_glTexParameterfv;
    extern ENGINE_GLAD_API PFNGLDRAWBUFFERPROC adr_glDrawBuffer;
    extern ENGINE_GLAD_API PFNGLREADBUFFERPROC adr_glReadBuffer;
    extern ENGINE_GLAD_API PFNGLFRAMEBUFFERTEXTUREPROC adr_glFramebufferTexture;
    extern ENGINE_GLAD_API PFNGLBLENDFUNCSEPARATEPROC adr_glBlendFuncSeparate;
    extern ENGINE_GLAD_API PFNGLDEPTHMASKPROC adr_glDepthMask;
    extern ENGINE_GLAD_API PFNGLDEPTHFUNCPROC adr_glDepthFunc;
}
