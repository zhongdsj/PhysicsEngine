# pragma once

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace ZDSJ {
	class MovementInterface;
	class DrawAbleData;
	typedef unsigned short      UINT16;
	struct Vertex2D;
	class DrawAbleInterface {
		friend class BaseManager;
	public:
		// virtual const char* getName() = 0;
		virtual void draw(ID3D11DeviceContext* _context, const DrawAbleData* _drawAbleData, bool _bind_static=true) = 0;
		virtual void click() = 0;
		virtual ~DrawAbleInterface() = default;
	protected:
		virtual void update(ID3D11DeviceContext* _context, const DrawAbleData* _drawAbleData) = 0;
		virtual void bind(ID3D11DeviceContext* _context) = 0;
		virtual void bindStatic(ID3D11DeviceContext* _context) = 0;
		virtual void drawIndex(ID3D11DeviceContext* _context, unsigned int _start_index_location, int _base_vertex_location) = 0;
	};
	
}