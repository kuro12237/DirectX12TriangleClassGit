#include "DebugTools.h"

DebugTools* DebugTools::GetInstance()
{
	static DebugTools instance;
	return &instance;
}

void DebugTools::addCommand(ICommand *command, const string& log)
{
	DebugTools::GetInstance()->commands.push_back(command);

	LogManager::Log(log+"\n");
}

void DebugTools::ClearCommand()
{
	DebugTools::GetInstance()->commands.clear();
}

ViewProjection DebugTools::ConvertViewProjection(ViewProjection& viewProjection)
{
	ViewProjection result{};

	if (!DebugTools::GetInstance()->isViewProjectionReWrite)
	{
		DebugTools::GetInstance()->viewProjection_ = viewProjection;
		result = viewProjection;

		if (!DebugTools::GetInstance()->isCameraSave)
		{
			DebugTools::GetInstance()->SaveViewProjection_ = viewProjection;
		}else if(DebugTools::GetInstance()->isCameraSave)
		{
			result = DebugTools::GetInstance()->SaveViewProjection_;
			DebugTools::GetInstance()->isCameraSave = false;
		}
	}
	else if (DebugTools::GetInstance()->isViewProjectionReWrite)
	{
		result = DebugTools::GetInstance()->viewProjection_;
		DebugTools::GetInstance()->isCameraSave = true;
	}
	

	return result;
}

void DebugTools::UpdateExecute(int buttonIndex)
{
	if (buttonIndex >= 0 && buttonIndex < DebugTools::GetInstance()->commands.size())
	{
		DebugTools::GetInstance()->commands[buttonIndex]->UpdateExecute();
	}
	else
	{
		LogManager::Log("None_Command\n");
		assert(0);
	}

}

void DebugTools::DrawExecute(int buttonIndex)
{
	if (buttonIndex >= 0 && buttonIndex < DebugTools::GetInstance()->commands.size()) 
	{
		DebugTools::GetInstance()->commands[buttonIndex]->DrawExecute(DebugTools::GetInstance()->viewProjection_);
	}
	else 
	{
		LogManager::Log("None_Command\n");
		assert(0);
	}

}
