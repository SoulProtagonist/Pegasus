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

#ifndef __SCENE_NODE__
#define __SCENE_NODE__

#define GLM_FORCE_RADIANS

#include <list>
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class SceneNode
{
private:
	std::list<SceneNode*> m_children;
	std::string m_name;
	std::string m_modelName;
	
	double m_rotVelocity;
	glm::vec3 m_rotAxis;
	glm::quat m_quat;
public:
	SceneNode(std::string node_name);
	SceneNode(const SceneNode& entity);
	~SceneNode();
	SceneNode& operator=(const SceneNode& entity);

	void AddChild(SceneNode* child);
	std::string GetName();
	glm::mat4 GetModelMatrix();
	void SetRotationalVelocity(double vel);
	void SetRotationalAxis(glm::vec3 axis);
	void SetName(std::string name);
	void Update(unsigned int delta);
	void UpdateAll(unsigned int delta);
	void SetModel(std::string model_name);
	void RemoveAllChildren();
	void RenderAll(glm::mat4 modelview, glm::mat4 projection);
};

#endif
