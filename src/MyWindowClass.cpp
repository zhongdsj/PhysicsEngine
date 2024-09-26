# include <MyWindowClass.h>
# include <MyWindowInterface.h>

ZDSJ::MyWindowClass::MyWindowClass(HINSTANCE _instance, LPCWSTR _class_name, UINT _style, HICON _icon, HICON _icon_sm, HCURSOR _cursor, LPCWSTR _menue_name, int _cls_extra, int _wnd_extra, HBRUSH _background)
{
	this->m_window_class.cbSize = sizeof(WNDCLASSEXW);
	this->m_window_class.lpfnWndProc = ZDSJ::MyWindowClass::handelMessageSetUp;
	this->m_window_class.hInstance = _instance;
	this->m_window_class.lpszClassName = _class_name;
	this->m_window_class.style = _style;
	this->m_window_class.hIcon = _icon;
	this->m_window_class.hIconSm = _icon_sm;
	this->m_window_class.hCursor = _cursor;
	this->m_window_class.lpszMenuName = _menue_name;
	this->m_window_class.cbClsExtra = _cls_extra;
	this->m_window_class.cbWndExtra = _wnd_extra;
	this->m_window_class.hbrBackground = _background;
	auto res = RegisterClassExW(&this->m_window_class);// ע�ᴰ����
}

bool ZDSJ::MyWindowClass::creatWindow(ZDSJ::MyWindowInterface* _window, int _x, int _y, int _width, int _height, LPCWSTR _window_name, HWND _parent, DWORD _ex_style, DWORD _style, HMENU _menu)
{
	return _window->create(_ex_style, this->m_window_class.lpszClassName, _window_name, _style, _x, _y, _width, _height, _parent, _menu, this->m_window_class.hInstance);
}

ZDSJ::MyWindowClass::~MyWindowClass()
{
	if (this->m_window_class.cbSize != 0) {
		UnregisterClassW(this->m_window_class.lpszClassName, this->m_window_class.hInstance);
	}
}

LRESULT ZDSJ::MyWindowClass::handelMessageSetUp(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_NCCREATE) {
		// ��������ʱ����
		// lParam ָ�� CREATESTRUCT �ṹ��ָ�룬���а����й����ڴ����Ĵ��ڵ���Ϣ��
		// CREATESTRUCT.lpCreateParams Ҳ����CreateWindowExW��LPVOIDָ���ָ��
		const CREATESTRUCTW* const pCreat = reinterpret_cast<CREATESTRUCTW*>(lParam);
		ZDSJ::MyWindowInterface* const pwnd = static_cast<ZDSJ::MyWindowInterface*>(pCreat->lpCreateParams);
		// ��ִ��CreateWindowExW��thisָ����봰��
		SetWindowLongPtrW(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pwnd));
		// ����ͨ��WINAPI���ó�Ա����, �ڽ��˺�������Ϊ��Ϣ�������󣬱�����WINAPI��
		// ����Ϣת������̬����
		// GWLP_WNDPROC���ô��ڹ��̵��µ�ַ
		SetWindowLongPtrW(handle, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&ZDSJ::MyWindowClass::handelMessageForward));
		return pwnd->handelMessage(handle, msg, wParam, lParam);
	}
	return DefWindowProc(handle, msg, wParam, lParam);
}

LRESULT ZDSJ::MyWindowClass::handelMessageForward(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	ZDSJ::MyWindowInterface* const pwnd = reinterpret_cast<ZDSJ::MyWindowInterface*>(GetWindowLongPtrW(handle, GWLP_USERDATA));
	return pwnd->handelMessage(handle, msg, wParam, lParam);
}
