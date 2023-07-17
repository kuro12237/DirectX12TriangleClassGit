#include"Input.h"



Input::Input()
{
}

Input::~Input()
{


}

Input* Input::Instance()
{
	static Input instance;
	return &instance;


}

void Input::DirectInputObjCreate()
{
	HRESULT hr;

	hr = DirectInput8Create(
		WinApp::GetInstance()->GetWc().hInstance,
		DIRECTINPUT_VERSION, IID_IDirectInput8,
		(void**)&Input::Instance()->diractInput,
		nullptr
	);
	assert(SUCCEEDED(hr));

}

void Input::KeyDeviceCreate()
{
	HRESULT hr =
		Input::Instance()->diractInput->CreateDevice(
			GUID_SysKeyboard, &Input::Instance()->InputDevice_.keybord, NULL
		);
	assert(SUCCEEDED(hr));

	hr = Input::Instance()->InputDevice_.keybord->SetDataFormat(&c_dfDIKeyboard);
	assert(SUCCEEDED(hr));

	hr = Input::Instance()->InputDevice_.keybord->SetCooperativeLevel(
		WinApp::GetInstance()->GetHwnd(), 
		DISCL_FOREGROUND |
		DISCL_NONEXCLUSIVE |
		DISCL_NOWINKEY
	);
	assert(SUCCEEDED(hr));

}

void Input::BeginFlame(BYTE key[256])
{

	Input::Instance()->InputDevice_.keybord->Acquire();

	BYTE key_[256] = {0};
	memcpy(key_, key, sizeof(key));


	Input::Instance()->InputDevice_.keybord->GetDeviceState(sizeof(key_), key_);

	if (key_[DIK_9])
	{
		OutputDebugStringA("hit9\n");
	}


}
