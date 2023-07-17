#pragma once

#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#include"../Win/WinApp.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

struct SInputDevice
{
	IDirectInputDevice8* keybord = nullptr;

};

class Input
{
public:
	Input();
	~Input();

	static Input* Instance();

	static void DirectInputObjCreate();

	static void KeyDeviceCreate();

	static void BeginFlame(BYTE key[256]);
private:


	IDirectInput8* diractInput = nullptr;

	SInputDevice InputDevice_;
};
