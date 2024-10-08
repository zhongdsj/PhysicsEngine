# include <MyDx11/BindAble/VertexShaderBindAble.h>
# include <d3d11.h>

ZDSJ::VertexShaderBindAble::VertexShaderBindAble(ID3D11Device* _device, const BYTE* _vertex_shader, size_t _vertex_shader_size)
{
	_device->CreateVertexShader(_vertex_shader, _vertex_shader_size, nullptr, this->m_vertex_shader.GetAddressOf());
}

void ZDSJ::VertexShaderBindAble::bind(ID3D11DeviceContext* _context)
{
	_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	_context->VSSetShader(this->m_vertex_shader.Get(), 0, 0);
}
