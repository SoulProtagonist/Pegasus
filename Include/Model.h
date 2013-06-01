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
#ifndef __MODEL_H__
#define __MODEL_H__

#include "Importer.hpp"
#include <string>
#include <vector>
#include "PegasusGL.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Model
{
private:
	struct VertexDesc
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec2 texture_coord;
		glm::vec3 tangent;
		glm::vec3 bitangent;
	};

	enum TextureType
	{
		DIFFUSE,
		NORMAL,
		SPECULAR,
		NUM_TEXTURE_TYPES
	};

	Assimp::Importer m_Importer;
	const aiScene * m_pScene;

	GLuint m_vertexBuffer;
	GLuint m_indexBuffer;

	Shader m_shader;

	std::vector<GLuint> m_textures;
	int m_numIndices;

	void LoadTexture(TextureType type, std::string model_filename);

public:
	Model(){
		m_pScene = NULL;
	}
	~Model(){};

	void LoadModel(std::string filename);
	void Render();
	void CleanUp();
};

#endif
