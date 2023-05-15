#include"WinSetup.h"


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



