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
}

void ZDSJ::IndexBufferBindAble::getData(ID3D11Device* _device, ID3D11DeviceContext* _context, void** _data, size_t& _size) const
{
	Microsoft::WRL::ComPtr<ID3D11Buffer> temp_buffer = nullptr;
	D3D11_BUFFER_DESC bd = {};
	this->m_buffer->GetDesc(&bd);
	bd.BindFlags = 0;
	bd.Usage = D3D11_USAGE_STAGING;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	_device->CreateBuffer(&bd, nullptr, temp_buffer.GetAddressOf());
	_context->CopyResource(temp_buffer.Get(), this->m_buffer.Get());
	D3D11_MAPPED_SUBRESOURCE target_map_res;
	UINT sub_resource = D3D11CalcSubresource(0, 0, 0);
	HRESULT hr = _context->Map(temp_buffer.Get(), sub_resource, D3D11_MAP_READ, 0, &target_map_res);
	_size = bd.ByteWidth;
	*_data = new byte[_size];
	if (SUCCEEDED(hr)) {

		memcpy_s(*_data, _size, target_map_res.pData, _size);
		_context->Unmap(temp_buffer.Get(), sub_resource);
	}
}

ZDSJ::IndexBufferBindAble::~IndexBufferBindAble()
{
}
