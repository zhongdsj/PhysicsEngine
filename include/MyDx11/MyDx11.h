#pragma once
# include <Windows.h>
# include <vector>

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;

namespace ZDSJ {
	#define SAFE_RELEASE(p) {if(p != nullptr){(p)->Release(); p=nullptr;}}
	class DrawAbleInterface;

	class MyDx11 {
	public:
		MyDx11(HWND _hwnd, int _window_width, int _window_height);
		void render();
		void endRender();
		ID3D11Device* device() const;
		ID3D11DeviceContext* context() const;
		~MyDx11();
	private:
		IDXGISwapChain* m_swap_chain = nullptr;
		ID3D11Device* m_device = nullptr;
		ID3D11DeviceContext* m_context = nullptr;
		ID3D11RenderTargetView* m_render_target_view = nullptr;
		std::vector<DrawAbleInterface*> m_draw_able;
		size_t m_index_size = 0;

		void draw();
		void createTriangle2D();
		// ID3D11Texture2D* m_texture = nullptr;
	};

}
