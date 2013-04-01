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

#include <GL/glew.h>
#include "GameRenderer.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <il.h>
#include <iostream>

GameRenderer::GameRenderer()
{
	ilInit();
}

struct vertex
{
	vertex(glm::vec3 p, glm::vec2 t) {
		pos = p;
		uv = t;
	}
	glm::vec3 pos;
	glm::vec2 uv;
};

void GameRenderer::Render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	Shader shader;
	shader.LoadShader("../Resources/Shaders/simple.vert", "../Resources/Shaders/simple.frag");
	shader.LinkShader();
	shader.EnableShader();

	GLuint array_buffer = 0;
	glGenBuffers(1, &array_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, array_buffer);

	std::vector<vertex> tri;
	tri.push_back(vertex(glm::vec3(-0.8f,  0.8f, 0.0f), glm::vec2(0.0f, 1.0f)));
	tri.push_back(vertex(glm::vec3(-0.8f, -0.8f, 0.0f), glm::vec2(0.0f, 0.0f)));
	tri.push_back(vertex(glm::vec3( 0.8f, -0.8f, 0.0f), glm::vec2(1.0f, 0.0f)));
	glBufferData(GL_ARRAY_BUFFER, tri.size() * sizeof(vertex), &tri[0], GL_STATIC_DRAW);
	glm::mat4 projection = glm::perspective(60.0f, 640.0f/480.0f, 0.1f, 100.0f);
	glm::mat4 model_view = glm::lookAt(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 mvp = projection * model_view;

	ilEnable(IL_ORIGIN_SET);
	ilSetInteger(IL_ORIGIN_MODE, IL_ORIGIN_LOWER_LEFT);
	ILenum ilerr = ilGetError();
	ILuint image = ilGenImage();
	ilBindImage(image);
	if(!ilLoadImage("../Resources/Textures/texture.jpg"))
	{
		std::cout << "image load failed";
	}
	ILubyte * data = new ILubyte[ilGetInteger(IL_IMAGE_WIDTH) * ilGetInteger(IL_IMAGE_HEIGHT) * 3];
	ilCopyPixels(0, 0, 0, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 1, IL_RGB, IL_UNSIGNED_BYTE, data); 
	glActiveTexture(GL_TEXTURE0);
	GLuint tex_id;
	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ilGetInteger(IL_IMAGE_WIDTH), ilGetInteger(IL_IMAGE_HEIGHT), 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	delete[] data; data = NULL;

	glUniformMatrix4fv(shader.GetUniform("mvp_matrix"), 1, GL_FALSE, glm::value_ptr(mvp));
	glUniform1i(shader.GetUniform("diffuse"), 0);
	glVertexAttribPointer(shader.GetAttribute("position"), 3, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)0);
	glVertexAttribPointer(shader.GetAttribute("texture_coord"), 2, GL_FLOAT, GL_FALSE, sizeof(vertex), (void*)sizeof(glm::vec3));
	glEnableVertexAttribArray(shader.GetAttribute("position"));
	glEnableVertexAttribArray(shader.GetAttribute("texture_coord"));

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &tex_id);
	glDeleteBuffers(1, &array_buffer);
	shader.DisableShader();
	shader.CleanupShader();
}