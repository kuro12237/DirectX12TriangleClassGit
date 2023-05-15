#include <Windows.h>


class WinSetup
{
public:
	WinSetup();
	~WinSetup();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const int Width,const int Height);



	/// <summary>
    ///ウインドウメッセージ処理 
    /// </summary>
    /// <param name="msg"></param>
	void WinMSG(MSG& msg);

	HWND SetHwnd()
	{
		return hwnd_;
	}

private:

	//ウインドウクラス
	WNDCLASS wc_{};
	HWND hwnd_;


};
