# include <MyDx11/BindAble/IndexBufferBindAble.h>
# include <d3d11.h>

ZDSJ::IndexBufferBindAble::IndexBufferBindAble(ID3D11Device* _device, const UINT16* _indices, size_t _indicesSize)
{
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.ByteWidth = _indicesSize;
	ibd.StructureByteStride = sizeof(UINT16);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = _indices;
	_device->CreateBuffer(&ibd, &isd, this->m_buffer.GetAddressOf());
}

void ZDSJ::IndexBufferBindAble::bind(ID3D11DeviceContext* _context)
{
	_context->IASetIndexBuffer(this->m_buffer.Get(), DXGI_FORMAT_R16_UINT, 0);
	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

ZDSJ::IndexBufferBindAble::~IndexBufferBindAble()
{
}
