# include <MyDx11/DrawAbleAdapter.h>
# include <MyDx11/BindAbleInterface.h>
# include <d3d11.h>

ZDSJ::DrawAbleAdapter::DrawAbleAdapter(ID3D11Device* _device) : m_bind_able(new std::vector<ZDSJ::BindAbleInterface*>)
{

}

void ZDSJ::DrawAbleAdapter::draw(ID3D11DeviceContext* _context)
{
	//if (this->m_index_size == 0) {
	//	return;
	//}

	_context->DrawIndexed(this->m_index_size, 0u, 0u);
}

ZDSJ::DrawAbleAdapter::~DrawAbleAdapter()
{
	delete this->m_bind_able;
}

void ZDSJ::DrawAbleAdapter::bind(ID3D11DeviceContext* _context)
{
	for (auto item : *this->m_bind_able) {
		item->bind(_context);
	}
}
