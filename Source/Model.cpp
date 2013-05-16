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

#include "Model.h"
#include "scene.h"
#include "postprocess.h"
#include <vector>
#include <assert.h>
#include <il.h>
#include "Log.h"

#define member_size(type, member) sizeof(((type*)0)->member)

void Model::LoadModel(std::string filename) {
	m_pScene = m_Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals);
	m_shader.LoadShader("../Resources/Shaders/model.vert", "../Resources/Shaders/model.frag");
	m_shader.LinkShader();
	m_shader.EnableShader();
	gl.GenBuffers(1, &m_indexBuffer);
	gl.GenBuffers(1, &m_vertexBuffer);
	std::vector<VertexDesc> vertex_data;
	std::vector<unsigned int> index_data;
	m_numIndices = 0;
	// there should only be a single mesh for a static object
	if(m_pScene->mNumMeshes != 1) {
		pegasusLog.Print("error models should have exactly one mesh");
	}
	const aiMesh* pMesh = m_pScene->mMeshes[0];
	m_numIndices = pMesh->mNumFaces * 3;
	for(unsigned int j = 0; j < pMesh->mNumVertices; j++) {
		const aiVector3D* pPos = &(pMesh->mVertices[j]);
		const aiVector3D* pNormal = &(pMesh->mNormals[j]);
		const aiVector3D* pTexCoord = &(pMesh->mTextureCoords[0][j]);

		VertexDesc vert;
		vert.position.x = pPos->x; vert.position.y = pPos->y; vert.position.z = pPos->z;
		vert.normal.x = pNormal->x; vert.normal.y = pNormal->y; vert.normal.z = pNormal->z;
		vert.texture_coord.x = pTexCoord->x; vert.texture_coord.y = pTexCoord->y;

		vertex_data.push_back(vert);
	}
	for(unsigned int j = 0; j < pMesh->mNumFaces; j++) {
		const aiFace* pFace = &(pMesh->mFaces[j]);
		assert(pFace->mNumIndices == 3);
		index_data.push_back(pFace->mIndices[0]);
		index_data.push_back(pFace->mIndices[1]);
		index_data.push_back(pFace->mIndices[2]);
	}
	
	std::string tex_loc = filename.replace(filename.end()-3, filename.end(), "tga");
	std::string diff_map = "_DiffuseMap";
	tex_loc.insert(tex_loc.end()-4, diff_map.begin(), diff_map.end());
	gl.GenTextures(1, &m_diffuseTexture);
	ilEnable(IL_ORIGIN_SET);
	ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_LOWER_LEFT);
	ILenum ilerr = ilGetError();
	ILuint image = ilGenImage();
	ilBindImage(image);
	if(!ilLoadImage(tex_loc.c_str())) {
		pegasusLog.Print("image load failed");
	}
	ILubyte * data = new ILubyte[ilGetInteger(IL_IMAGE_WIDTH) * ilGetInteger(IL_IMAGE_HEIGHT) * 3];
	ilCopyPixels(0, 0, 0, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 1, IL_RGB, IL_UNSIGNED_BYTE, data); 
	gl.BindTexture(GL_TEXTURE_2D, m_diffuseTexture);
	gl.TexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	delete[] data; data = NULL;	

	gl.BindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	gl.BufferData(GL_ARRAY_BUFFER, sizeof(vertex_data[0]) * vertex_data.size(), &(vertex_data[0]), GL_STATIC_DRAW);

	gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	gl.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data[0]) * index_data.size(), &(index_data[0]), GL_STATIC_DRAW);

	glm::mat4 projection = glm::perspective(60.0f, 640.0f/480.0f, 0.5f, 1000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(50.0f, 10.0f, 20.0f), glm::vec3(0.0f, 10.0f, -35.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mvp = projection * view;
	gl.UniformMatrix4fv(m_shader.GetUniform("mvp_matrix"), 1, GL_FALSE, glm::value_ptr(mvp));
	gl.UniformMatrix4fv(m_shader.GetUniform("mv_matrix"), 1, GL_FALSE, glm::value_ptr(view));
}

void Model::Render(){
	m_shader.EnableShader();

	gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	gl.BindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	gl.VertexAttribPointer(m_shader.GetAttribute("position"), 3, GL_FLOAT, GL_FALSE, sizeof(VertexDesc), (void*)0);
	gl.VertexAttribPointer(m_shader.GetAttribute("normal"), 3, GL_FLOAT, GL_FALSE, sizeof(VertexDesc), (void*)member_size(VertexDesc, position));
	gl.VertexAttribPointer(m_shader.GetAttribute("texture_coord"), 2, GL_FLOAT, GL_FALSE, sizeof(VertexDesc), (void*)(member_size(VertexDesc, position) + member_size(VertexDesc, normal)));
	gl.EnableVertexAttribArray(m_shader.GetAttribute("position"));
	gl.EnableVertexAttribArray(m_shader.GetAttribute("normal"));
	gl.EnableVertexAttribArray(m_shader.GetAttribute("texture_coord"));
	gl.ActiveTexture(GL_TEXTURE0);
	gl.Uniform1i(m_shader.GetUniform("diffuse"), 0);
	gl.TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gl.TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gl.BindTexture(GL_TEXTURE_2D, m_diffuseTexture);
	gl.DrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, (void*)0);
	gl.DisableVertexAttribArray(m_shader.GetAttribute("position"));
	gl.DisableVertexAttribArray(m_shader.GetAttribute("normal"));
}

void Model::CleanUp() {
	m_shader.DisableShader();
	m_shader.CleanupShader();
	gl.DeleteBuffers(1, &m_vertexBuffer);
	gl.DeleteBuffers(1, &m_indexBuffer);
}
