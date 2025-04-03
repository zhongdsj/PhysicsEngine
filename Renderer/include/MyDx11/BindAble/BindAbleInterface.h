#pragma once
# include <wrl/client.h>

struct ID3D11DeviceContext;
struct ID3D11Device;

namespace ZDSJ {

	class BindAbleInterface {
	public:
		virtual void bind(ID3D11DeviceContext* _context) = 0;
	};

}