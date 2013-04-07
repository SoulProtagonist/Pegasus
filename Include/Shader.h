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

#ifndef SHADER_H_
#define SHADER_H_

#include <string>
#include "PegasusGL.h"

class Shader
{
private:
	unsigned int vertex_shader;
	unsigned int fragment_shader;
	unsigned int program;

	void LoadShaderSource(const std::string shader_file, const unsigned int shader_id);
	static void GetStringFromFile(const std::string file_path, char** ret_string, int* size);
public:
	Shader(): vertex_shader(0), fragment_shader(0), program(0) {}
	~Shader(){}

	void LoadShader(const std::string vert_path, const std::string frag_path);
	void LinkShader() const;
	void EnableShader() const;
	void DisableShader() const;
	void CleanupShader();

	int GetAttribute(const std::string attrib_name) const;
	int GetUniform(const std::string uniform_name) const;
};

#endif