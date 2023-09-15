#pragma once
#include"WorldTransform.h"

class Model;
class IModelState
{
public:
	virtual ~IModelState() {};
	virtual void Initialize(Model *state) = 0;
	virtual void Draw(Model* state,WorldTransform worldTransform,ViewProjection viewprojection) = 0;
private:

};

