#pragma once
# include <windows.h>
# include <vector>
# include <functional>

struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Texture2D;
struct ID3D11RenderTargetView;
struct ID3D11RasterizerState;

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
		void solid();
		void wireframe();
		ID3D11Device* device() const;
		ID3D11DeviceContext* context() const;
		~MyDx11();
	private:
		IDXGISwapChain* m_swap_chain = nullptr;
		ID3D11Device* m_device = nullptr;
		ID3D11DeviceContext* m_context = nullptr;
		ID3D11RenderTargetView* m_render_target_view = nullptr;
		DrawAbleManager* m_drawable_manager = nullptr;
		ID3D11RasterizerState* m_solid_rasterizer_state = nullptr;
		ID3D11RasterizerState* m_wireframe_rasterizer_state = nullptr;
		size_t m_index_size = 0;

		float m_background[4] = { 0.0f, 0.0f, 0.0f, 1.0f };

		void clearByBackground();
		void createTriangle2D(float _x, float _y, float _width, float _height);
		void createRectangle2D(float _x, float _y, float _width, float _height);
		void createCircle2D(float _x, float _y, float _width, float _height);
		std::function<void(float, float, float, float)> m_create = std::bind(&MyDx11::createCircle2D, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
		float m_create_width = 20.0f;
		float m_create_height = 20.0f;
		// ID3D11Texture2D* m_texture = nullptr;
	};

}
