# pragma once
# include <MyDx11/BindAble/BindAbleInterface.h>
struct ID3D11Buffer;

namespace ZDSJ {

	class IndexBufferBindAble : public BindAbleInterface {
	public:
		IndexBufferBindAble(ID3D11Device* _device, const UINT16* _indices, size_t _indicesSize);
		void bind(ID3D11DeviceContext* _context) override;
		~IndexBufferBindAble();
	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer = nullptr;
	};

}