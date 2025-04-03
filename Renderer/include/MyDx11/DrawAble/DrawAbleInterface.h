# pragma once
# include <Types.h>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace ZDSJ {
	class MovementInterface;

	struct Vertex2D;
	typedef unsigned short      UINT16;

	class DrawAbleInterface {
		friend class DrawAbleManager;
	public:
		virtual void draw(ID3D11DeviceContext* _context, bool _bind_static=true) = 0;
		virtual void click() = 0;
		virtual bool pointInPolgon2D(float _x, float _y) = 0;
		virtual void calculateForce(DrawAbleInterface* _other) = 0;
		virtual void moveByAcceleration() = 0;
		virtual char* save(size_t& _size) = 0;
		virtual const float3& position() const = 0;
		virtual const float3& size() const = 0;
		virtual MovementInterface* getMovement() const = 0;
		virtual ~DrawAbleInterface() {};
	protected:
		virtual void update(ID3D11DeviceContext* _context) = 0;
		virtual void bind(ID3D11DeviceContext* _context) = 0;
		virtual void bindStatic(ID3D11DeviceContext* _context) = 0;
		virtual void drawIndex(ID3D11DeviceContext* _context, unsigned int _start_index_location, int _base_vertex_location) = 0;
		
	};
	
}