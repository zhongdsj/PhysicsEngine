# pragma once
# include <vector>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace ZDSJ {

	class DrawAbleInterface {
		friend class DrawAbleManager;
	public:
		virtual void draw(ID3D11DeviceContext* _context) = 0;
	protected:
		virtual void update(ID3D11DeviceContext* _context) = 0;
		virtual void bind(ID3D11DeviceContext* _context) = 0;
		virtual void bindStatic(ID3D11DeviceContext* _context) = 0;
		virtual void drawIndex(ID3D11DeviceContext* _context, unsigned int _start_index_location, int _base_vertex_location) = 0;
	};

}