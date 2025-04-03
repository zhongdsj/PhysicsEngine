# pragma once
# include <atomic>
# include <ConsoleInterface.h>
# include <ConsoleExport.h>
# include <CommonMacro.h>
# include <memory>
# include <vector>

struct ID3D11Device;
struct IDXGISwapChain;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

namespace ZDSJ
{
	class Slot;
	class Console_Api ConsoleByImgui : public ConsoleInterface
	{
	public:
		ConsoleByImgui(HWND _handle);
		Delete_Copy_And_Move(ConsoleByImgui);
		void tick(float _use_time) override;
		LRESULT messageHandle(HWND _handle, UINT _msg, WPARAM _w_param, LPARAM _l_param) override;
		bool show() override;
		~ConsoleByImgui() override;
	private:
		int m_console_flag = 0;
		ID3D11Device* m_device;
		ID3D11DeviceContext* m_context;
		std::atomic_bool m_show{false};
		std::vector<Slot*> m_slots;

		void rend();
	};

}
