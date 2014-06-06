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

#define GLM_FORCE_RADIANS

#include <SDL.h>
#include "PegasusGL.h"
#include "GameRenderer.h"
#include "Shader.h"
#include <vector>
#include <il.h>
#include <iostream>
#include "Model.h"
#include "ModelManager.h"
#include "Log.h"
#include "SceneManager.h"
#include "SceneNode.h"

GameRenderer::GameRenderer()
{
}

void GameRenderer::Setup()
{
	pegasusLog.Print("Starting renderer");
	ilInit();
	gl.LoadFunctions();
	gl.ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	gl.Enable(GL_DEPTH_TEST);
	gl.ClearDepth(1.0);
	ModelManager::GetInst()->LoadModels();
}

void GameRenderer::Render()
{
	gl.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 projection = glm::perspective(glm::radians(60.0f), 640.0f/480.0f, 0.5f, 1000.0f);
        glm::mat4 view = glm::lookAt(glm::vec3(0.0f,30.0f, 60.0f), glm::vec3(0.0f, 10.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	SceneManager::GetInst()->GetRoot()->RenderAll(view, projection);
}

void GameRenderer::CleanUp()
{
	ModelManager::GetInst()->GetModel("MODEL1")->CleanUp();
}
