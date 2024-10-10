# pragma once
# include <MyDx11/DrawAble/DrawAbleAdapter.h>
# include <MyDx11/Animations/DrawAbleAnimation.h>
# include <vector>

struct ID3D11Buffer;

namespace ZDSJ {
	class BindAbleInterface;

	class Rectangle2DDrawAble : public DrawAbleAdapter {
	public:
		Rectangle2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context, const DrawAbleData& _data);
		Rectangle2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context);
		~Rectangle2DDrawAble();
	};
}