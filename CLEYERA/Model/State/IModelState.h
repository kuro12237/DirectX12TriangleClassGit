#pragma once
#include"WorldTransform.h"

class Model;
class IModelState
{
public:
	virtual ~IModelState() {};
	virtual void Initialize(Model *state) = 0;
	virtual void Draw(Model* state,WorldTransform worldTransform) = 0;
private:

};

