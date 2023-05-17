#include"WinSetup.h"
#include<string>
#include<format>

class Adapter
{
public:
	Adapter();
	~Adapter();

	void Initialize(const int32_t kClientWidth, const int32_t  kClientHeight);

	void WinMSG( MSG &msg);

	void BeginFlame();

	void EndFlame();

	void Deleate();

private:


	WindowsSetup* WinSetup_ = new WindowsSetup();



};



