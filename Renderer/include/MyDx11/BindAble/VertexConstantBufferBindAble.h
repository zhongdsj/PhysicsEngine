# pragma once
# include <MyDx11/BindAble/ConstantBufferBindAble.h>

namespace ZDSJ {

	class VertexConstantBufferBindAble : public ConstantBufferBindAble {
	public:
		VertexConstantBufferBindAble(ID3D11Device* _device);
		void bind(ID3D11DeviceContext* _context) override;
		void update(ID3D11DeviceContext* _context, const DirectX::XMMATRIX& _update_buffer) override;
	};

}