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

#include "SceneManager.h"

SceneManager* SceneManager::m_instance = NULL;

SceneManager::SceneManager()
{
	m_root = NULL;
}

SceneManager::SceneManager(const SceneManager& mgr)
{
	m_root = mgr.m_root;
	m_instance = mgr.m_instance;
}

SceneManager& SceneManager::operator=(const SceneManager& mgr)
{
	m_root = mgr.m_root;
	m_instance = mgr.m_instance;
}

SceneManager::~SceneManager()
{
	m_root->RemoveAllChildren();

	delete m_root;
	delete m_instance;

	m_root = NULL;
	m_instance = NULL;
}

SceneManager* SceneManager::GetInst()
{
	if(m_instance == NULL)
	{
		m_instance = new SceneManager();
	}

	return m_instance;
}


SceneNode* SceneManager::GetRoot()
{
	if(m_root == NULL)
	{
		m_root = new SceneNode("root");
	}

	return m_root;
}
