# include <MyDx11/PixelShaderBindAble.h>
# include <d3d11.h>

ZDSJ::PixelShaderBindAble::PixelShaderBindAble(ID3D11Device* _device, const BYTE* _pixel_shader, size_t _pixel_shader_size)
{
	_device->CreatePixelShader(_pixel_shader, _pixel_shader_size, nullptr, this->m_pixelShader.GetAddressOf());
}

void ZDSJ::PixelShaderBindAble::bind(ID3D11DeviceContext* _context)
{
	_context->PSSetShader(this->m_pixelShader.Get(), 0, 0);
}
