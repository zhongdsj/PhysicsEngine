# include <MyDx11/VertexConstantBufferBindAble.h>
# include <d3d11.h>
# include <DirectXMath.h>

ZDSJ::VertexConstantBufferBindAble::VertexConstantBufferBindAble(ID3D11Device* _device) : ZDSJ::ConstantBufferBindAble(_device)
{
}

void ZDSJ::VertexConstantBufferBindAble::bind(ID3D11DeviceContext* _context)
{
	_context->VSSetConstantBuffers(0u, 1u, this->m_buffer.GetAddressOf());
}

void ZDSJ::VertexConstantBufferBindAble::update(ID3D11DeviceContext* _context, const DirectX::XMMATRIX& _update_buffer)
{
	D3D11_MAPPED_SUBRESOURCE msr;
	_context->Map(this->m_buffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &msr);
	memcpy_s(msr.pData, sizeof(_update_buffer), &_update_buffer, sizeof(_update_buffer));
	_context->Unmap(this->m_buffer.Get(), 0u);
}
