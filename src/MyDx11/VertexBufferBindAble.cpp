# include <MyDx11/VertexBufferBindAble.h>

void ZDSJ::VertexBufferBindAble::bind(ID3D11DeviceContext* _context)
{
	_context->IASetVertexBuffers(0, 1, this->m_buffer.GetAddressOf(), &this->m_stride, &this->m_offset);
}
