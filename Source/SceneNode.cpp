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

#include "SceneNode.h"
#include "ModelManager.h"
#include "Log.h"
#include <sstream>

SceneNode::SceneNode(std::string node_name):m_name(node_name),m_modelName("none"), m_rotVelocity(0.0), m_rotAxis(glm::vec3(0.0f, 1.0f, 0.0f)), m_quat()
{
}

SceneNode::SceneNode(const SceneNode& entity)
{
	// children are not copied
	m_name = entity.m_name;
	m_modelName = entity.m_modelName;
	m_rotVelocity = entity.m_rotVelocity;
	m_rotAxis = entity.m_rotAxis;
}

SceneNode::~SceneNode()
{
	RemoveAllChildren();
}

SceneNode& SceneNode::operator=(const SceneNode& entity)
{
	// children are not copied
	m_name = entity.m_name;
	m_modelName = entity.m_modelName;
	m_rotVelocity = entity.m_rotVelocity;
	m_rotAxis = entity.m_rotAxis;
}

void SceneNode::AddChild(SceneNode* child)
{
	m_children.push_back(child);
}

std::string SceneNode::GetName()
{
	return m_name;
}

glm::mat4 SceneNode::GetModelMatrix()
{
	return glm::mat4_cast(m_quat);
}

void SceneNode::SetRotationalVelocity(double vel)
{
	m_rotVelocity = vel;
}

void SceneNode::SetRotationalAxis(glm::vec3 axis)
{
	m_rotAxis = axis;
}

void SceneNode::SetName(std::string name)
{
	m_name = name;
}

void SceneNode::Update(unsigned int delta)
{
	double delta_seconds = delta/1000.0;
	m_quat = glm::rotate(m_quat, float(m_rotVelocity * delta_seconds), m_rotAxis);	
}

void SceneNode::UpdateAll(unsigned int delta)
{
	Update(delta);
	for(std::list<SceneNode*>::iterator iter = m_children.begin(); iter != m_children.end(); iter++)
	{
		(*iter)->UpdateAll(delta);
	}
}

void SceneNode::SetModel(std::string model_name)
{
	m_modelName = model_name;
}

void SceneNode::RemoveAllChildren()
{
	for(std::list<SceneNode*>::iterator iter = m_children.begin(); iter != m_children.end(); iter++)
	{
		(*iter)->RemoveAllChildren();
		delete *iter;
	}

	m_children.clear();
}

void SceneNode::RenderAll(glm::mat4 modelview, glm::mat4 projection)
{
	glm::mat4 mv = modelview * GetModelMatrix();

	if(m_modelName.compare("none")!=0)
	{
		ModelManager::GetInst()->GetModel(m_modelName)->Render(projection, mv);
	}

	for(std::list<SceneNode*>::iterator iter = m_children.begin(); iter != m_children.end(); iter++)
	{
		(*iter)->RenderAll(mv, projection);
	}
}
