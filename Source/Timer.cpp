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

#include "Timer.h"
#include <SDL.h>

Timer::Timer():m_started(false), m_startTime(0) 
{}

Timer::~Timer()
{}

Timer::Timer(const Timer& other):m_started(other.m_started), m_startTime(other.m_startTime)
{
}

Timer& Timer::operator=(const Timer& other)
{
	m_started = other.m_started;
	m_startTime = other.m_startTime;
}

void Timer::Start()
{
	m_started = true;
	m_startTime = SDL_GetTicks();
}

void Timer::Stop()
{
	m_started = false;
}

unsigned int Timer::GetTime() const
{
	if(m_started)
	{
		return SDL_GetTicks() - m_startTime;
	}

	// if not started return zero
	return 0;
}

bool Timer::IsStarted() const
{
	return m_started;
}

void Timer::Wait(const unsigned int time) const
{
	unsigned int currentTime = GetTime();
	while(GetTime() < (currentTime + time))
	{
		// loops until time has passed
	}
}
