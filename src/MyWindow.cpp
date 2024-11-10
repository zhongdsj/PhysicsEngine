# include <MyWindow.h>
# include <MyDx11/MyDx11.h>
# include <ImGuiManager/ImGuiManager.h>
# include <MyDx11/DrawAbleManager.h>
# include <MyDx11/Context.h>
# include <Timer.h>

# pragma region MyWindow

HWND ZDSJ::MyWindow::getHwnd()
{
	return this->m_hwnd;
}

bool ZDSJ::MyWindow::create(DWORD _ex_style, LPCWSTR _class_name, LPCWSTR _window_name, DWORD _style, int _x, int _y, int _width, int _height, HWND _parent, HMENU _menu, HINSTANCE _instance)
{
	this->m_hwnd = CreateWindowExW(
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
	ZDSJ::Context::getInstance()->windowWidth(_width)->windowHeight(_height);
	if (this->m_hwnd == nullptr) {
		auto err = GetLastError();
		return false;
	}
	this->m_dx11 = new ZDSJ::MyDx11(this->m_hwnd, _width, _height, ZDSJ::RenderType::Category);
	this->m_imgui = new ZDSJ::ImGuiManager(this->m_hwnd, this->m_dx11->device(), this->m_dx11->context());
	ShowWindow(this->m_hwnd, SW_SHOWDEFAULT);

	return true;
}

LRESULT ZDSJ::MyWindow::handelMessage(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (this->m_imgui != nullptr) {
		if (this->m_imgui->handelMessage(handle, msg, wParam, lParam)) {
			return true;
		}
	}
	switch (msg)
	{
	case WM_MOUSEMOVE:
		if (wParam == MK_LBUTTON) {
			short x = static_cast<short>(LOWORD(lParam));
			short y = static_cast<short>(HIWORD(lParam));
			ZDSJ::Context::getInstance()->keyboard()->splitFloatToShorts(lParam, x, y);
			ZDSJ::Context::getInstance()->keyboard()->mouseDrag(x, y);
			// ZDSJ::Context::getInstance()->Keyboard()->execKeyboard(WM_MOUSEMOVE, lParam);
		}
		ZDSJ::Context::getInstance()->keyboard()->execKeyboard(WM_MOUSEMOVE, lParam);
		break;
	case WM_LBUTTONUP:
		ZDSJ::Context::getInstance()->keyboard()->mouseDragReset();
		break;
	case WM_LBUTTONDOWN:
		ZDSJ::Context::getInstance()->keyboard()->execKeyboard(VK_LBUTTON, lParam);
		break;
	case WM_RBUTTONDOWN:
		ZDSJ::Context::getInstance()->keyboard()->execKeyboard(VK_RBUTTON, lParam);
		break;
	case WM_SYSKEYDOWN:
		ZDSJ::Context::getInstance()->keyboard()->mod(VK_LMENU);
		break;
	case WM_SYSKEYUP:
		switch (wParam)
		{
		case VK_CONTROL:
			ZDSJ::Context::getInstance()->keyboard()->mod(ZDSJ::Key::nothing);
			return true;
			// break;
		default:
			break;
		}
		ZDSJ::Context::getInstance()->keyboard()->mod(ZDSJ::Key::nothing);
		break;
	case WM_MOUSEWHEEL:
		ZDSJ::Context::getInstance()->keyboard()->execKeyboard(WM_MOUSEWHEEL, static_cast<short>(HIWORD(wParam))/120);
		break;
	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_CONTROL:
			ZDSJ::Context::getInstance()->keyboard()->mod(VK_CONTROL);
			break;
		
		default:
			ZDSJ::Context::getInstance()->keyboard()->execKeyboard(wParam, 0.0f);
			break;
		}
		break;
	case WM_KEYUP:
		switch (wParam) {
		case VK_CONTROL:
			ZDSJ::Context::getInstance()->keyboard()->mod(ZDSJ::Key::nothing);
			break;;
		default:
			break;
		}
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProc(handle, msg, wParam, lParam);
}

void ZDSJ::MyWindow::changeFps(short _fps)
{
	if (this->m_rend_state) {
		this->m_rend_state = false;
		this->m_rend_thread->join();
		delete this->m_rend_thread;
		delete this->m_fps_control;
	}
	this->m_rend_state = true;
	ZDSJ::Context::getInstance()->fps(_fps);
	this->m_fps_control = new ZDSJ::Timer(_fps);
	this->m_rend_thread = new std::thread(&ZDSJ::MyWindow::rend, this);
}

void ZDSJ::MyWindow::run(short _fps)
{
	this->changeFps(_fps);
	MSG msg;
	while (GetMessage(&msg, nullptr, 0, 0)) {
		if (msg.message == WM_QUIT)
		{
			break;
		}
		else {
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
		}
	}
}

void ZDSJ::MyWindow::doFrame()
{
	
	this->m_dx11->render();
	if (this->m_imgui != nullptr) {
		this->m_imgui->render();
	}
	this->m_dx11->endRender();
}

ZDSJ::MyWindow::~MyWindow()
{
	if (this->m_rend_state) {
		this->m_rend_state = false;
		this->m_rend_thread->join();
		delete this->m_rend_thread;
		delete this->m_fps_control;
	}
	delete this->m_imgui;
	delete this->m_dx11;
	DestroyWindow(this->m_hwnd);
}

void ZDSJ::MyWindow::rend()
{
	while (this->m_rend_state) {
		this->m_fps_control->mark();
		this->doFrame();
		this->m_fps_control->nextFps();
	}
}

# pragma endregion MyWindow
