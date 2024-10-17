# pragma once
# include <windows.h>
# include <functional>
# include <RingBuffer.h>

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ImGuiInputTextCallbackData;

namespace ZDSJ {
	class ImGuiManager {
	public:
		ImGuiManager(HWND _hwnd, ID3D11Device* _device, ID3D11DeviceContext* _context);
		LRESULT handelMessage(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam);
		void render();
		
		~ImGuiManager();
	private:
		bool m_show_fps = false;
		bool m_show_console = false;
		bool m_auto_console_bottom = false;
		int m_window_flag = 0;

		void showFps(bool _show);

		static int textEditCallbackStub(ImGuiInputTextCallbackData* _data);
	};

}
