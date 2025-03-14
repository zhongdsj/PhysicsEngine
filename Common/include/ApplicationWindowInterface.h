# pragma once
# include "CommonExport.h"
# include <functional>
# include <wtypes.h>
# include <CommonMacro.h>
# include <SignalSender.h>

namespace ZDSJ
{
	class Common_Api ApplicationWindowInterface : public SignalSender
	{
	public:
		ApplicationWindowInterface() = default;
		Delete_Copy_And_Move(ApplicationWindowInterface);
		virtual HWND getHandle() const = 0;
		virtual void addHandelMessage(const char* _id, std::function<LRESULT(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param)> _handler) = 0;
		virtual bool getMessage(MSG& _msg) = 0;
		virtual ~ApplicationWindowInterface() = default;
	protected:
		void registerToContext();
	};

	Common_Api ZDSJ::ApplicationWindowInterface* createWindow(const wchar_t* _window_name, int _x, int _y, int _width, int _height);
}
