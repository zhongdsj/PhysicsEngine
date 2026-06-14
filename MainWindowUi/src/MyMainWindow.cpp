# include <MyMainWindow.h>
# include <Context.h>
# include <MyWindowClass.h>
# include <windowsx.h>

ZDSJ::SpecialKey win32ToSpecialKey(WPARAM w_param)
{
	switch (w_param)
	{
	case VK_CONTROL: return ZDSJ::SpecialKey::key_ctrl;
	case VK_MENU:    return ZDSJ::SpecialKey::key_alt;
	case VK_OEM_3:   return ZDSJ::SpecialKey::key_wavy;
	case 'T':		 return ZDSJ::SpecialKey::key_t;
	case 'R':		 return ZDSJ::SpecialKey::key_r;
	default:         return ZDSJ::SpecialKey::max_keys;
	}
}

ZDSJ::MyMainWindow::MyMainWindow(DWORD _ex_style, LPCWSTR _class_name, LPCWSTR _window_name, DWORD _style, int _x,
                                 int _y, int _width, int _height, HWND _parent, HMENU _menu, HINSTANCE _instance)
{
	// TODO 窗口位置与大小改为类自身读取配置文件
	this->m_windowClass = std::make_shared<MyWindowClass>(_instance, _class_name);
	this->m_handle = CreateWindowExW(
		_ex_style,
		// WS_EX_TOPMOST: 置顶, 调试ui时可用
		// WS_EX_ACCEPTFILES: 允许拖拽文件
		// WS_EX_TOOLWINDOW: 该窗口打算用作浮动工具栏,不会被alt+tab呼出,如果有菜单则右键呼出
		// WS_EX_NOACTIVATE: 可以让窗体被点击后 也不激活，不抢夺焦点，同时窗口上的输入框也不会默认获得输入焦点
		// WS_EX_LAYERED
		// WS_EX_TRANSPARENT
		// WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP
		_class_name,                     // Window class
		_window_name,    // Window text
		_style, // WS_POPUPWINDOW: 没有标题栏
		_x, _y, _width, _height, _parent, _menu, _instance, this
	);
	ShowWindow(this->m_handle, SW_SHOWDEFAULT);
	this->MyMainWindow::registerSignal();
	this->registerToContext();
}

ZDSJ::Slot* ZDSJ::MyMainWindow::connect(const std::string& signal, Slot* slot)
{
	auto pair = this->m_slots.find(signal);
	if(pair != this->m_slots.end())
	{
		std::get<1>(pair->second).push_back(slot);
	}
	return slot;
}

LRESULT ZDSJ::MyMainWindow::handleMessage(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param)
{
	// todo 处理消息
	for (auto message_handler : this->m_messageHandler)
	{
		if(message_handler.second(handle, msg, w_param, l_param))
		{
			return true;
		}
	}
	switch (msg)
	{
	case WM_LBUTTONDOWN:
		Context_Instance->getKeyBoard()->setKeyState(SpecialKey::mouse_left, KeyOperation::down);
		break;
	case WM_LBUTTONUP:
		Context_Instance->getKeyBoard()->setKeyState(SpecialKey::mouse_left, KeyOperation::up);
		break;
	case WM_MOUSEMOVE:
		{
			auto point = Point(GET_X_LPARAM(l_param), GET_Y_LPARAM(l_param));
			Context_Instance->setMouse(point);
		}
		break;
	case WM_KEYUP:
		Context_Instance->getKeyBoard()->setKeyState(win32ToSpecialKey(w_param), KeyOperation::up);
		break;
	case WM_KEYDOWN:
		Context_Instance->getKeyBoard()->setKeyState(win32ToSpecialKey(w_param), KeyOperation::down);
		break;
	case WM_MOVE:
		{
			const int x_pos = (int)static_cast<short>(LOWORD(l_param));   // horizontal position 
			const int y_pos = (int)static_cast<short>(HIWORD(l_param));   // vertical position 
			Context_Instance->setWindowX(x_pos);
			Context_Instance->setWindowY(y_pos);
			break;
		}
	case WM_MOUSEWHEEL:
		Keyboard_Instance->addAxisStates(AxisKey::mouse_wheel, GET_WHEEL_DELTA_WPARAM(w_param));
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(handle, msg, w_param, l_param);
}

void ZDSJ::MyMainWindow::addHandleMessage(const char* _id, std::function<LRESULT(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param)> _handler)
{
	this->m_messageHandler.insert(std::make_pair(_id, _handler));
}

void ZDSJ::MyMainWindow::removeHandleMessage(const char* _id)
{
	auto pair = this->m_messageHandler.find(_id);
	if(pair != this->m_messageHandler.end())
	{
		this->m_messageHandler.erase(pair);
	}
}

bool ZDSJ::MyMainWindow::getMessage(MSG& _msg)
{
	// 处理消息, 退出/异常消息返回false，外部视情况处理
	BOOL res = PeekMessage(&_msg, nullptr, 0U, 0U, PM_REMOVE);
	if(res == 0)
	{
		return true;
	}
	if (_msg.message == WM_QUIT)
	{
		return false;
	}
	TranslateMessage(&_msg);
	DispatchMessageW(&_msg);
	return true;
}

HWND ZDSJ::MyMainWindow::getHandle() const
{
	return this->m_handle;
}

ZDSJ::MyMainWindow::~MyMainWindow()
{
	DestroyWindow(this->m_handle);
	Log_Info("[win api]: window destroy");
}

void ZDSJ::MyMainWindow::registerSignal()
{
}

ZDSJ::ApplicationWindowInterface* ZDSJ::createWindow(const wchar_t* _window_name, int _x, int _y, int _width, int _height)
{
	Log_Info("[win api]: create window");
	constexpr DWORD ex_style = NULL;
	const auto class_name = L"ZDSJWindow";
	// TODO 改为创建父窗口
	// WS_CLIPCHILDREN	0x02000000L	在父窗口内进行绘图时，不包括子窗口所占用的区域。 创建父窗口时使用此样式。
	constexpr DWORD style = WS_SYSMENU;
	const HWND parent = nullptr;
	const HMENU menu = nullptr;
	const HINSTANCE instance = GetModuleHandle(nullptr);
	auto window = new MyMainWindow(ex_style, class_name, _window_name, style, _x, _y, _width, _height, parent, menu, instance);
	Log_Info("[win api]: window created");
	return window;
}
