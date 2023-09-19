#pragma once
#include"Pch.h"
#include"WinApp.h"


class Input
{
public:
	Input();
	~Input();

	static Input * GetInstance();

	static void Initialize();

	static void BeginFlame();

	bool PushKey(uint8_t keyNum);

	bool PushKeyPressed(uint32_t keyNum);

private:

	static void CreateKeybordDevice();

	ComPtr<IDirectInput8> directInput = nullptr;
	ComPtr<IDirectInputDevice8> keyboard = nullptr;

	BYTE keys[256] = {};
	BYTE preKeys[256] = {};
	//0x80=‰Ÿ‚µ‚Ä‚¢‚éó‘Ô
	//0x00=‰Ÿ‚µ‚Ä‚È‚¢ó‘Ô
};

