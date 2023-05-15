#include"WinSetup.h"


class Adapter
{
public:
	Adapter();
	~Adapter();

	void Initialize(const int Width, const int Height);




private:


	WinSetup* WinSetup_ = new WinSetup();



};

Adapter::Adapter()
{
}

Adapter::~Adapter()
{
}


