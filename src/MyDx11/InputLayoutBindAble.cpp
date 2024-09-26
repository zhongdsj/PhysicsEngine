# include <MyDx11/InputLayoutBindAble.h>
# include <d3d11.h>

ZDSJ::InputLayoutBindAble::InputLayoutBindAble(ID3D11Device* _device, std::vector<D3D11_INPUT_ELEMENT_DESC>& _ied, const BYTE* _vertex_shader, size_t _vertex_shader_size)
{
	_device->CreateInputLayout(_ied.data(), _ied.size(), _vertex_shader, _vertex_shader_size, this->m_inputLayout.GetAddressOf());
}

void ZDSJ::InputLayoutBindAble::bind(ID3D11DeviceContext* _context)
{
	_context->IASetInputLayout(this->m_inputLayout.Get());
}
