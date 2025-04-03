# pragma once
# include <MyDx11/DrawAble/DrawAbleBase.h>

namespace ZDSJ {

	class BindAbleInterface;

	class Arc2DDrawAble : public DrawAbleBase<Arc2DDrawAble> {
	public:
		Arc2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context, int _divisions, float _rotate, const DrawAbleData& _data);
		Arc2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context, int _divisions, float _rotate);
		Arc2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context, int _divisions, float _rotate, float3 _size, float3 _pos);
		~Arc2DDrawAble();
	};

}