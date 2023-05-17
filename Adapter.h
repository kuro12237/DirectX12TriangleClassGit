#include"WinSetup.h"
#include <windows.h>
#include<string>
#include<format>
class Adapter
{
public:
	Adapter();
	~Adapter();

	void Initialize(const int Width, const int Height);

	void WinMSG( MSG &msg);




private:


	WindowsSetup* WinSetup_ = new WindowsSetup();



};



