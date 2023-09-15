#pragma once
#include"ViewProjection.h"

class ICommand
{
public:

	virtual ~ICommand() {};
	virtual void execute(ViewProjection viewProjedction) = 0;

};

