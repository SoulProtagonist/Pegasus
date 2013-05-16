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

#include <SDL.h>
#include "PegasusGL.h"
#include "GameRenderer.h"
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <il.h>
#include <iostream>
#include <Model.h>
#include "Log.h"

GameRenderer::GameRenderer()
{
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

void GameRenderer::Setup()
{
	pegasusLog.Print("Starting renderer");
	ilInit();
	gl.LoadFunctions();
	gl.ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	gl.Enable(GL_DEPTH_TEST);
	gl.ClearDepth(1.0);
	model.LoadModel("../Resources/RoadCone/RoadCone.obj");
}

void GameRenderer::Render()
{
	gl.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	model.Render();
}

void GameRenderer::CleanUp()
{
	model.CleanUp();
}
