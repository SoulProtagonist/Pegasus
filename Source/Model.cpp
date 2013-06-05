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
	m_pScene = m_Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
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
	if(pMesh->HasTangentsAndBitangents() == false) {
		pegasusLog.Print("model doesn't have tangents");
	}
	for(unsigned int j = 0; j < pMesh->mNumVertices; j++) {
		const aiVector3D* pPos = &(pMesh->mVertices[j]);
		const aiVector3D* pNormal = &(pMesh->mNormals[j]);
		const aiVector3D* pTexCoord = &(pMesh->mTextureCoords[0][j]);
		const aiVector3D* pTangent = &(pMesh->mTangents[j]);
		const aiVector3D* pBitangent = &(pMesh->mBitangents[j]);

		VertexDesc vert;
		vert.position.x = pPos->x; vert.position.y = pPos->y; vert.position.z = pPos->z;
		vert.normal.x = pNormal->x; vert.normal.y = pNormal->y; vert.normal.z = pNormal->z;
		vert.texture_coord.x = pTexCoord->x; vert.texture_coord.y = pTexCoord->y;
		vert.tangent.x = pTangent->x; vert.tangent.y = pTangent->y; vert.tangent.z = pTangent->z;
		vert.bitangent.x = pBitangent->x; vert.bitangent.y = pBitangent->y; vert.bitangent.z = pBitangent->z;

		vertex_data.push_back(vert);
	}
	for(unsigned int j = 0; j < pMesh->mNumFaces; j++) {
		const aiFace* pFace = &(pMesh->mFaces[j]);
		assert(pFace->mNumIndices == 3);
		index_data.push_back(pFace->mIndices[0]);
		index_data.push_back(pFace->mIndices[1]);
		index_data.push_back(pFace->mIndices[2]);
	}
	
	m_textures.resize(NUM_TEXTURE_TYPES);
	gl.GenTextures(NUM_TEXTURE_TYPES, &m_textures[0]);
	gl.ActiveTexture(GL_TEXTURE0);
	LoadTexture(DIFFUSE, filename);
	gl.ActiveTexture(GL_TEXTURE1);
	LoadTexture(NORMAL, filename);
	gl.ActiveTexture(GL_TEXTURE2);
	LoadTexture(SPECULAR, filename);

	gl.BindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	gl.BufferData(GL_ARRAY_BUFFER, sizeof(vertex_data[0]) * vertex_data.size(), &(vertex_data[0]), GL_STATIC_DRAW);

	gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	gl.BufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index_data[0]) * index_data.size(), &(index_data[0]), GL_STATIC_DRAW);

	glm::mat4 projection = glm::perspective(60.0f, 640.0f/480.0f, 0.5f, 1000.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(50.0f, 10.0f, 20.0f), glm::vec3(0.0f, 10.0f, -35.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mvp = projection * view;
	glm::mat3 norm = glm::transpose(glm::inverse(glm::mat3(view)));
	gl.UniformMatrix4fv(m_shader.GetUniform("mvp_matrix"), 1, GL_FALSE, glm::value_ptr(mvp));
	gl.UniformMatrix4fv(m_shader.GetUniform("mv_matrix"), 1, GL_FALSE, glm::value_ptr(view));
	gl.UniformMatrix3fv(m_shader.GetUniform("n_matrix"), 1, GL_FALSE, glm::value_ptr(norm));
}

void Model::Render(){
	m_shader.EnableShader();

	gl.BindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer);
	gl.BindBuffer(GL_ARRAY_BUFFER, m_vertexBuffer);
	gl.VertexAttribPointer(m_shader.GetAttribute("position"), 3, GL_FLOAT, GL_FALSE, sizeof(VertexDesc), (void*)0);
	gl.VertexAttribPointer(m_shader.GetAttribute("normal"), 3, GL_FLOAT, GL_FALSE, sizeof(VertexDesc), (void*)member_size(VertexDesc, position));
	gl.VertexAttribPointer(m_shader.GetAttribute("texture_coord"), 2, GL_FLOAT, GL_FALSE, sizeof(VertexDesc), (void*)(member_size(VertexDesc, position) + member_size(VertexDesc, normal)));
	gl.VertexAttribPointer(m_shader.GetAttribute("tangent"), 3, GL_FLOAT, GL_FALSE, sizeof(VertexDesc), (void*)(member_size(VertexDesc, position) + member_size(VertexDesc, normal) + member_size(VertexDesc, texture_coord)));
	gl.VertexAttribPointer(m_shader.GetAttribute("bitangent"), 3, GL_FLOAT, GL_FALSE, sizeof(VertexDesc), (void*)(member_size(VertexDesc, position) + member_size(VertexDesc, normal) + member_size(VertexDesc, texture_coord) + member_size(VertexDesc, tangent)));
	gl.EnableVertexAttribArray(m_shader.GetAttribute("position"));
	gl.EnableVertexAttribArray(m_shader.GetAttribute("normal"));
	gl.EnableVertexAttribArray(m_shader.GetAttribute("texture_coord"));
	gl.EnableVertexAttribArray(m_shader.GetAttribute("tangent"));
	gl.EnableVertexAttribArray(m_shader.GetAttribute("bitangent"));
	gl.Uniform1i(m_shader.GetUniform("diffuseMap"), 0);
	gl.Uniform1i(m_shader.GetUniform("normalMap"), 1);
	gl.ActiveTexture(GL_TEXTURE0);
	gl.BindTexture(GL_TEXTURE_2D, m_textures[DIFFUSE]);
	gl.TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gl.TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gl.ActiveTexture(GL_TEXTURE1);
	gl.BindTexture(GL_TEXTURE_2D, m_textures[NORMAL]);
	gl.TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gl.TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gl.ActiveTexture(GL_TEXTURE2);
	gl.BindTexture(GL_TEXTURE_2D, m_textures[SPECULAR]);
	gl.TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	gl.TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	gl.DrawElements(GL_TRIANGLES, m_numIndices, GL_UNSIGNED_INT, (void*)0);
	gl.DisableVertexAttribArray(m_shader.GetAttribute("position"));
	gl.DisableVertexAttribArray(m_shader.GetAttribute("normal"));
	gl.DisableVertexAttribArray(m_shader.GetAttribute("texture_coord"));
	gl.DisableVertexAttribArray(m_shader.GetAttribute("tangent"));
	gl.DisableVertexAttribArray(m_shader.GetAttribute("bitangent"));
}

void Model::CleanUp() {
	m_shader.DisableShader();
	m_shader.CleanupShader();
	gl.DeleteBuffers(1, &m_vertexBuffer);
	gl.DeleteBuffers(1, &m_indexBuffer);
}

void Model::LoadTexture(TextureType type, std::string model_filename) {
	std::string tex_loc = model_filename.replace(model_filename.end()-3, model_filename.end(), "tga");
	std::vector<std::string> map_names;
	map_names.resize(3);
	map_names[DIFFUSE] = "_DiffuseMap";
	map_names[NORMAL] = "_NormalsMap";
	map_names[SPECULAR] = "_SpecularityMap";
	if(type < DIFFUSE || type > SPECULAR) {
		pegasusLog.Print("Texture type not recognised. Failed to load texture");
		return;
	}
	tex_loc.insert(tex_loc.end()-4, map_names[type].begin(), map_names[type].end());
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
	gl.BindTexture(GL_TEXTURE_2D, m_textures[type]);
	gl.TexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	delete[] data; data = NULL;
}
