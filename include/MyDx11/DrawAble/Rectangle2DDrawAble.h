# pragma once
# include <MyDx11/DrawAble/DrawAbleBase.h>
# include <MyDx11/Animations/DrawAbleAnimation.h>

struct ID3D11Buffer;

namespace ZDSJ {
	class BindAbleInterface;

	class Rectangle2DDrawAble : public DrawAbleBase<Rectangle2DDrawAble> {
	public:
		Rectangle2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context, const DrawAbleData& _data);
		Rectangle2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context);
		~Rectangle2DDrawAble();
	};
}