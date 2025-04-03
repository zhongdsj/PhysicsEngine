# pragma once
# include <CommonExport.h>
# include <wtypes.h>
# include <ComponentInterface.h>

namespace ZDSJ
{
	class Common_Api ConsoleInterface: public ComponentInterface
	{
	public:
		virtual bool show() = 0;
		virtual LRESULT messageHandle(HWND _handle, UINT _msg, WPARAM _w_param, LPARAM _l_param) = 0;
		virtual ~ConsoleInterface() override = default;
	};
	Common_Api ZDSJ::ConsoleInterface* createConsole(HWND _handle);
}

