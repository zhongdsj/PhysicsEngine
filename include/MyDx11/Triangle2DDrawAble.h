# pragma once
# include <MyDx11/DrawAbleAdapter.h>

namespace ZDSJ {
	class BindAbleInterface;

	class Triangle2DDrawAble : public DrawAbleAdapter {
	public:
		Triangle2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context);
		~Triangle2DDrawAble() = default;
	};
}