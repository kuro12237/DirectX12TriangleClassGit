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




private:


	WindowsSetup* WinSetup_ = new WindowsSetup();



};



