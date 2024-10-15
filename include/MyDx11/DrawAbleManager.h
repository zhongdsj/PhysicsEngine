# pragma once

struct ID3D11DeviceContext;

namespace ZDSJ {

	class DrawAbleInterface;

	enum RenderType
	{
		Default = 1,
		Category = 2
	};

	class DrawAbleManager {
	public:
		DrawAbleManager(RenderType _render_type = Default);

		void add(DrawAbleInterface* _drawable);

		void render(ID3D11DeviceContext* _context);

		~DrawAbleManager();
	private:
		RenderType m_render_type = Default;
		void* m_container = nullptr;

		void defaultRender(ID3D11DeviceContext* _context);
		void CategoryRender(ID3D11DeviceContext* _context);

		void vectorContainerAdd(DrawAbleInterface* _drawable);
		void mapContainerAdd(DrawAbleInterface* _drawable);

		void vectorContainerClear();
		void mapContainerClear();
	};
}