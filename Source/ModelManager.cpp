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

#include "Model.h"
#include "ModelManager.h"

ModelManager* ModelManager::m_instance = NULL;

ModelManager::ModelManager()
{
}

ModelManager::ModelManager(const ModelManager& mgr)
{
}

ModelManager::~ModelManager()
{
}

void ModelManager::AddModel(std::string modelName, Model* model)
{
	m_modelMap[modelName] = model;
}

void ModelManager::RemoveModel(std::string modelName)
{
	Model* model = m_modelMap[modelName];
	if(model != NULL)
		delete model;

	m_modelMap.erase(modelName);
}

void ModelManager::LoadModels()
{
	for(std::map<std::string, Model*>::iterator iter = m_modelMap.begin(); iter != m_modelMap.end(); iter++)
	{
		iter->second->Initialise();
	}
}

Model* ModelManager::GetModel(std::string modelName)
{
	return m_modelMap[modelName];
}

ModelManager* ModelManager::GetInst()
{
	if(m_instance == NULL)
	{
		m_instance = new ModelManager();
	}

	return m_instance;
}
