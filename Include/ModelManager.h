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

#ifndef __MODEL_MGR__
#define __MODEL_MGR__

#include "Model.h"
#include <map>
#include <string>

class ModelManager
{
private:
	std::map<std::string, Model*> m_modelMap;
	static ModelManager* m_instance;
	ModelManager();
	ModelManager(const ModelManager& mgr);

public:
	~ModelManager();
	void AddModel(std::string modelName, Model* model);
	void RemoveModel(std::string modelName);
	void LoadModels();
	Model* GetModel(std::string modelName);

	static ModelManager* GetInst();
};

#endif
