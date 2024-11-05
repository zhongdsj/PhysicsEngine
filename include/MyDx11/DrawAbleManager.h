# pragma once
# include <ostream>
# include <vector>

struct ID3D11DeviceContext;

namespace ZDSJ {

	struct Point;
	class DrawAbleInterface;

	enum class RenderType
	{
		Default,
		Category,
	};

	struct Vertex2D;
	typedef unsigned short UINT16;

	class DrawAbleManager {
		friend class Persistence;
	public:
		explicit DrawAbleManager(const RenderType _render_type = RenderType::Default);

		void add(DrawAbleInterface* _drawable);

		void render(ID3D11DeviceContext* _context);

		~DrawAbleManager();
	private:
		RenderType m_render_type = RenderType::Default;
		void* m_container = nullptr;

		char* save(size_t& _size) const;

		void itemSave(ZDSJ::DrawAbleInterface* _item, std::ostringstream* _oss, size_t& _size) const;
		char* defaultSave(size_t& _size) const;
		char* categorySave(size_t& _size) const;

		void itemRender(std::vector<ZDSJ::DrawAbleInterface*>::iterator& _iterator, ID3D11DeviceContext* _context, std::vector<ZDSJ::DrawAbleInterface*>& _container, const ZDSJ::Point& _mouse_word, bool _draw_static);
		void defaultRender(ID3D11DeviceContext* _context);
		void categoryRender(ID3D11DeviceContext* _context);

		void vectorContainerAdd(DrawAbleInterface* _drawable);
		void mapContainerAdd(DrawAbleInterface* _drawable);

		void vectorContainerClear();
		void mapContainerClear();
	};
}