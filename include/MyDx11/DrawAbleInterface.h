# pragma once
# include <vector>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace ZDSJ {

	class DrawAbleInterface {
	public:
		virtual void draw(ID3D11DeviceContext* _context) = 0;
	};

}