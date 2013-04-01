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

#include "Shader.h"
#include <SDL.h>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <sstream>

void Shader::LoadShader(std::string vert_shader, std::string frag_shader)
{
	vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	// load shader objects into opengl
	LoadShaderSource(vert_shader, vertex_shader);
	LoadShaderSource(frag_shader, fragment_shader);

	glCompileShader(vertex_shader);
	glCompileShader(fragment_shader);

	GLint result;
	glGetShaderiv(vertex_shader, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE)
	{
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;

		glGetShaderiv(vertex_shader, GL_INFO_LOG_LENGTH,&infologLength);

		if (infologLength > 0)
		{
			infoLog = (char *)malloc(infologLength);
			glGetShaderInfoLog(vertex_shader, infologLength, &charsWritten, infoLog);
			printf("%s\n",infoLog);
			free(infoLog);
		}
	}
	glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &result);
	if(result == GL_FALSE)
	{
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;

		glGetShaderiv(fragment_shader, GL_INFO_LOG_LENGTH,&infologLength);

		if (infologLength > 0)
		{
			infoLog = (char *)malloc(infologLength);
			glGetShaderInfoLog(fragment_shader, infologLength, &charsWritten, infoLog);
			printf("%s\n",infoLog);
			free(infoLog);
		}
	}

	program = glCreateProgram();
	glAttachShader(program, vertex_shader);
	glAttachShader(program, fragment_shader);
}

void Shader::LinkShader() const
{
	glLinkProgram(program);

	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if(result == GL_FALSE)
	{
		int infologLength = 0;
		int charsWritten  = 0;
		char *infoLog;

		glGetProgramiv(program, GL_INFO_LOG_LENGTH,&infologLength);

		if (infologLength > 0)
		{
			infoLog = (char *)malloc(infologLength);
			glGetProgramInfoLog(program, infologLength, &charsWritten, infoLog);
			printf("%s\n",infoLog);
			free(infoLog);
		}
	}
}

void Shader::EnableShader() const
{
	glUseProgram(program);
}

void Shader::DisableShader() const
{
	glUseProgram(0);
}

void Shader::CleanupShader()
{
	glDetachShader(program, vertex_shader);
	glDetachShader(program, fragment_shader);
	glDeleteProgram(program);
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	program = 0;
	vertex_shader = 0;
	fragment_shader = 0;
}

int Shader::GetAttribute(const std::string attrib_name) const
{
	return glGetAttribLocation(program, attrib_name.c_str());
}

int Shader::GetUniform(const std::string uniform_name) const
{
	return glGetUniformLocation(program, uniform_name.c_str());
}

void Shader::GetStringFromFile(const std::string file_path, char** ret_string, int* size)
{
	std::ifstream file;
	std::stringstream ss;
	file.open(file_path, std::fstream::in);
	ss << file.rdbuf();

	*size = ss.str().size() + 1;
	*ret_string = new char[*size];

	strcpy(*ret_string, ss.str().c_str());
}

void Shader::LoadShaderSource(const std::string shader_file, const unsigned int shader_id)
{
	// open shader file
	char * shader_source = NULL;
	int size = 0;

	GetStringFromFile(shader_file, &shader_source, &size);
	glShaderSource(shader_id, 1, (const char**)&shader_source, &size);

	delete[] shader_source;
}