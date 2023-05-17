#include"WinSetup.h"
#include<string>
#include<format>

class Adapter
{
public:
	Adapter();
	~Adapter();

	void Initialize(const int32_t Width, const int32_t Height);

	void WinMSG( MSG &msg);

	void BeginFlame();

	void EndFlame();

	void Deleate();

private:


	WindowsSetup* WinSetup_ = new WindowsSetup();



};



