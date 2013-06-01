/*
=============================================================================

Pegasus GPL Source Code
Copyright (C) 2012 Leo Walsh

This file is part of the Pegasus GPL Source Code.  

Pegasus Source Code is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Pegasus Source Code is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Pegasus Source Code.  If not, see <http://www.gnu.org/licenses/>.

==============================================================================
*/

#ifndef __PEGASUSGL_H__
#define __PEGASUSGL_H__
#include <SDL_opengl.h>

typedef void (APIENTRYP PFNGLCLEARCOLORPROC) (GLclampf red, GLclampf green, GLclampf blue, GLclampf alpha);
typedef void (APIENTRYP PFNGLCLEARDEPTHPROC) (GLclampd depth);
typedef void (APIENTRYP PFNGLCLEARPROC) (GLbitfield mask);
typedef void (APIENTRYP PFNGLGENTEXTURESPROC) (GLsizei n, GLuint *textures);
typedef void (APIENTRYP PFNGLBINDTEXTUREPROC) (GLenum target, GLuint texture);
typedef void (APIENTRYP PFNGLTEXIMAGE2DPROC) (GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const GLvoid *pixels);
typedef void (APIENTRYP PFNGLTEXPARAMETERFPROC) (GLenum target, GLenum pname, GLfloat param);
typedef void (APIENTRYP PFNGLDRAWARRAYSPROC) (GLenum mode, GLint first, GLsizei count);
typedef void (APIENTRYP PFNGLDRAWELEMENTSPROC) (GLenum mode, GLsizei count, GLenum type, GLvoid* indices);
typedef void (APIENTRYP PFNGLDELETETEXTURESPROC) (GLsizei n, const GLuint *textures);
typedef void (APIENTRYP PFNGLENABLEPROC) (GLenum cap);

class PegasusGL
{
public:
	PFNGLGENBUFFERSPROC GenBuffers;
	PFNGLBINDBUFFERPROC BindBuffer;
	PFNGLBUFFERDATAPROC BufferData;
	PFNGLUNIFORMMATRIX4FVPROC UniformMatrix4fv;
	PFNGLUNIFORMMATRIX3FVPROC UniformMatrix3fv;
	PFNGLUNIFORM1IPROC Uniform1i;
	PFNGLVERTEXATTRIBPOINTERPROC VertexAttribPointer;
	PFNGLENABLEVERTEXATTRIBARRAYPROC EnableVertexAttribArray;
	PFNGLDELETEBUFFERSPROC DeleteBuffers;
	PFNGLCREATESHADERPROC CreateShader;
	PFNGLCOMPILESHADERPROC CompileShader;
	PFNGLGETSHADERIVPROC GetShaderiv;
	PFNGLGETSHADERINFOLOGPROC GetShaderInfoLog;
	PFNGLCREATEPROGRAMPROC CreateProgram;
	PFNGLATTACHSHADERPROC AttachShader;
	PFNGLLINKPROGRAMPROC LinkProgram;
	PFNGLGETPROGRAMIVPROC GetProgramiv;
	PFNGLGETPROGRAMINFOLOGPROC GetProgramInfoLog;
	PFNGLUSEPROGRAMPROC UseProgram;
	PFNGLDETACHSHADERPROC DetachShader;
	PFNGLDELETEPROGRAMPROC DeleteProgram;
	PFNGLDELETESHADERPROC DeleteShader;
	PFNGLGETATTRIBLOCATIONPROC GetAttribLocation;
	PFNGLGETUNIFORMLOCATIONPROC GetUniformLocation;
	PFNGLSHADERSOURCEPROC ShaderSource;
	PFNGLACTIVETEXTUREPROC ActiveTexture;
	PFNGLCLEARCOLORPROC ClearColor;
	PFNGLCLEARPROC Clear;
	PFNGLGENTEXTURESPROC GenTextures;
	PFNGLBINDTEXTUREPROC BindTexture;
	PFNGLTEXIMAGE2DPROC TexImage2D;
	PFNGLTEXPARAMETERFPROC TexParameterf;
	PFNGLDRAWARRAYSPROC DrawArrays;
	PFNGLDELETETEXTURESPROC DeleteTextures;
	PFNGLDRAWELEMENTSPROC DrawElements;
	PFNGLENABLEPROC Enable;
	PFNGLCLEARDEPTHPROC ClearDepth;
	PFNGLDISABLEVERTEXATTRIBARRAYPROC DisableVertexAttribArray;

	PegasusGL(){};
	virtual ~PegasusGL(){};
	void LoadFunctions();
};

extern PegasusGL gl;

#endif
