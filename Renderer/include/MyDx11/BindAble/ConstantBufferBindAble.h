# pragma once
# include <MyDx11/BindAble/BindAbleInterface.h>

struct ID3D11Buffer;
// struct XMMATRIX;
namespace DirectX {
	struct XMMATRIX;
}

namespace ZDSJ {
	class ConstantBufferBindAble : public BindAbleInterface {
	public:
		ConstantBufferBindAble(ID3D11Device* _device);
		void bind(ID3D11DeviceContext* _context) override;
		virtual void update(ID3D11DeviceContext* _context, const DirectX::XMMATRIX& _update_buffer) = 0;
		~ConstantBufferBindAble() = default;
	protected:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer = nullptr;
	};

}