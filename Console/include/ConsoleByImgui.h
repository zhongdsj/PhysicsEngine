# pragma once
# include <ConsoleInterface.h>
# include <ConsoleExport.h>
# include <CommonMacro.h>
# include <memory>

struct ID3D11Device;
struct IDXGISwapChain;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;

namespace ZDSJ
{
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
		std::shared_ptr<IDXGISwapChain> m_swap_chain;
		std::shared_ptr<ID3D11Device> m_device;
		std::shared_ptr<ID3D11DeviceContext> m_context;
		std::shared_ptr<ID3D11RenderTargetView> m_render_target_view;
		void rend();

		void createDeviceD3D(HWND _handle);
		void createRenderTarget();
	};

	ConsoleInterface* createConsole(HWND _handle)
	{
		return new ConsoleByImgui(_handle);
	}
}
