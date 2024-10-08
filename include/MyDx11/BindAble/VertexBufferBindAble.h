#pragma once
# include <MyDx11/BindAble/BindAbleInterface.h>
# include <vector>
# include <d3d11.h>

struct ID3D11Buffer;


namespace ZDSJ {
	class VertexBufferBindAble : public BindAbleInterface {
	public:
		template<class T>
		VertexBufferBindAble(ID3D11Device* _device, std::vector<T>& _vertices) {
				D3D11_BUFFER_DESC bd = {};
				bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
				bd.StructureByteStride = sizeof(T);
				bd.ByteWidth = bd.StructureByteStride * _vertices.size();
				D3D11_SUBRESOURCE_DATA sd = {};
				sd.pSysMem = _vertices.data();
				_device->CreateBuffer(&bd, &sd, this->m_buffer.GetAddressOf());
				this->m_stride = bd.StructureByteStride;
		}
		void bind(ID3D11DeviceContext* _context) override;
		~VertexBufferBindAble() = default;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer = nullptr;
		UINT m_stride = 0u;
		UINT m_offset = 0u;
	};

}
