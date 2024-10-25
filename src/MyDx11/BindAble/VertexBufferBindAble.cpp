# include <MyDx11/BindAble/VertexBufferBindAble.h>

void ZDSJ::VertexBufferBindAble::bind(ID3D11DeviceContext* _context)
{
	_context->IASetVertexBuffers(0, 1, this->m_buffer.GetAddressOf(), &this->m_stride, &this->m_offset);
}

void ZDSJ::VertexBufferBindAble::getData(ID3D11Device* _device, ID3D11DeviceContext* _context, void** _data, size_t& _size) const
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
