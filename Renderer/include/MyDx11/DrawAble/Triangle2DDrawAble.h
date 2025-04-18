# pragma once
# include <MyDx11/DrawAble/DrawAbleBase.h>

namespace ZDSJ {
	class BindAbleInterface;

	class Triangle2DDrawAble : public DrawAbleBase<Triangle2DDrawAble> {
	public:
		Triangle2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context);
		~Triangle2DDrawAble();
	};
}