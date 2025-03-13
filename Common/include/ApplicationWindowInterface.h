# pragma once
# include "CommonExport.h"
# include <functional>
# include <wtypes.h>

namespace ZDSJ
{
	class Common_Api ApplicationWindowInterface
	{
	public:
		virtual HWND getHandle() const = 0;
		virtual void addHandelMessage(const char* _id, std::function<LRESULT(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param)> _handler) = 0;
		virtual bool getMessage(MSG& _msg) = 0;
		virtual ~ApplicationWindowInterface() = default;
	};

	Common_Api ZDSJ::ApplicationWindowInterface* createWindow(const wchar_t* _window_name, int _x, int _y, int _width, int _height);
}
