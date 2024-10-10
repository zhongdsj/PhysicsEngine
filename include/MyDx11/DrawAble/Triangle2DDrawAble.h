# pragma once
# include <MyDx11/DrawAble/DrawAbleAdapter.h>
# include <MyDx11/Animations/DrawAbleAnimation.h>

namespace ZDSJ {
	class BindAbleInterface;

	class Triangle2DDrawAble : public DrawAbleAdapter {
	public:
		Triangle2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context, const DrawAbleData& _data);
		Triangle2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context);
		~Triangle2DDrawAble();
	};
}