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

#include "Engine.h"
#include <SDL.h>
#include <GL/glew.h>

void Engine::SetupWindow()
{
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_SetVideoMode(640, 480, 32, SDL_OPENGL);

	glewInit();
}

void Engine::CloseWindow()
{
	SDL_Quit();
}

void Engine::GameLoop()
{
	SDL_Event evt;
	int running = true;
	while(running)
	{
		if(SDL_PollEvent(&evt))
		{
			switch(evt.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			}
		}
		else
		{
			m_renderer->Render();
			SDL_GL_SwapBuffers();
		}
	}
}

void Engine::Run()
{
	if(!m_renderer)
		return;

	SetupWindow();
	GameLoop();
	CloseWindow();
	
	CleanUpResources();
}

void Engine::RegisterRenderer(IRenderer * renderer)
{
	if(m_renderer)
	{
		delete m_renderer;
		m_renderer = 0;
	}

	m_renderer = renderer;
}

void Engine::CleanUpResources()
{
	if(m_renderer)
	{
		delete m_renderer;
		m_renderer = 0;
	}
}