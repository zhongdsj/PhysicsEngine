# pragma once
# include <MainWindowUiExport.h>

namespace ZDSJ
{
	class Main_Window_Ui_Api MyWindowInterface
	{
	public:
		virtual LRESULT handleMessage(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param) = 0;
	};
}
