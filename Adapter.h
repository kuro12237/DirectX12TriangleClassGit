#include"WinSetup.h"
#include <windows.h>
#include<string>
#include<format>
class Adapter
{
public:
	Adapter();
	~Adapter();

	void Initialize(const int32_t kClientWidth, const int32_t  kClientHeight);

	void WinMSG( MSG &msg);




private:


	WindowsSetup* WinSetup_ = new WindowsSetup();



};



