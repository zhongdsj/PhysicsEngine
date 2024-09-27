# include <MyDx11/ConstantBufferBindAble.h>
# include <DirectXMath.h>
# include <d3d11.h>

ZDSJ::ConstantBufferBindAble::ConstantBufferBindAble(ID3D11Device* _device)
{
	DirectX::XMMATRIX matrix = DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 1.0f);
	matrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f), matrix);
	matrix = DirectX::XMMatrixMultiply(DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f), matrix);
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(matrix);
	cbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &matrix;
	_device->CreateBuffer(&cbd, &csd, this->m_buffer.GetAddressOf());
}

void ZDSJ::ConstantBufferBindAble::bind(ID3D11DeviceContext* _context)
{

}
