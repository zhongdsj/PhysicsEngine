#pragma once
# include <MyWindowInterface.h>
# include <thread>
# include <atomic>

namespace ZDSJ {
	class MyDx11;
	class ImGuiManager;
	class Timer;

	class MyWindow : public MyWindowInterface
	{
	public:
		MyWindow() = default;
		HWND getHwnd();
		bool create(DWORD _ex_style, LPCWSTR _class_name, LPCWSTR _window_name, DWORD _style, int _x, int _y, int _width, int _height,
			HWND _parent, HMENU _menu, HINSTANCE _instance) override;
		LRESULT handelMessage(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		void changeFps(short _fps);
		void run(short _fps);
		~MyWindow();
		MyWindow(const MyWindow&) = delete;
		MyWindow& operator=(const MyWindow&) = delete;
	private:
		HWND m_hwnd = nullptr;
		ImGuiManager* m_imgui = nullptr;
		MyDx11* m_dx11 = nullptr;

		Timer* m_fps_control = nullptr;
		std::thread* m_rend_thread = nullptr;
		std::atomic<bool> m_rend_state = false;
		void rend();
		void doFrame();
	};
}