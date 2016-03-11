#pragma once

#include "Model.h"

class Scene
{
public:
	Scene(int _numModels);
	~Scene();

private:
	int numModels;
	Model* models;

};

