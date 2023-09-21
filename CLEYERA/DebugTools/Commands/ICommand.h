#pragma once
#include"ViewProjection.h"

class ICommand
{
public:

	virtual ~ICommand() {};
	virtual void UpdateExecute() = 0;
	virtual void DrawExecute(ViewProjection viewProjedction) = 0;

};

