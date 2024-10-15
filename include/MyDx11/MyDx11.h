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
	class DrawAbleManager;
	enum RenderType;

	class MyDx11 {
	public:
		MyDx11(HWND _hwnd, int _window_width, int _window_height, RenderType _render_type);
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
		DrawAbleManager* m_drawable_manager = nullptr;
		size_t m_index_size = 0;

		float m_background[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		void clearByBackground();
		void createTriangle2D();
		// ID3D11Texture2D* m_texture = nullptr;
	};

}
