# pragma once
# include "CommonExport.h"

namespace ZDSJ
{
	class Common_Api ApplicationWindowInterface
	{
	public:
		virtual bool getMessage(MSG& _msg) = 0;
		virtual ~ApplicationWindowInterface() = default;
	};

	Common_Api ZDSJ::ApplicationWindowInterface* createWindow(const wchar_t* _window_name, int _x, int _y, int _width, int _height);
}
