# include <MyDx11/DrawAble/DrawAbleAdapter.h>
# include <MyDx11/BindAble/BindAbleInterface.h>
# include <d3d11.h>
# include <DirectXMath.h>

ZDSJ::DrawAbleAdapter::DrawAbleAdapter(ID3D11Device* _device) : m_bind_able(new std::vector<ZDSJ::BindAbleInterface*>)
{
}

void ZDSJ::DrawAbleAdapter::draw(ID3D11DeviceContext* _context, short _fps)
{
	//if (this->m_index_size == 0) {
	//	return;
	//}
	this->bind(_context);
	this->update(_context, _fps);
	_context->DrawIndexed(this->m_index_size, 0u, 0u);
}

ZDSJ::DrawAbleAdapter::~DrawAbleAdapter()
{
	delete this->m_bind_able;
}

void ZDSJ::DrawAbleAdapter::update(ID3D11DeviceContext* _context, short _fps)
{
}

void ZDSJ::DrawAbleAdapter::bind(ID3D11DeviceContext* _context)
{
	for (auto item : *this->m_bind_able) {
		item->bind(_context);
	}
}

DirectX::XMMATRIX ZDSJ::DrawAbleAdapter::getTransformMatix() const
{
	/*DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f, 1.0f, 1.0f);
	DirectX::XMMATRIX pro = DirectX::XMMatrixScaling(3.0f/4.0f, 1.0f, 1.0f);*/
	/*auto base = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	DirectX::XMMatrixRotationRollPitchYawFromVector*/
	DirectX::XMMATRIX pro = DirectX::XMMatrixOrthographicLH(2.0f * 800.0f/600.0f, 2.0f, 0.0f, 1.0f); // *2.0f;
	DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslation(this->m_translation_x, this->m_translation_y, this->m_translation_z)
		* DirectX::XMMatrixScaling(this->m_scal_x, this->m_scal_y, this->m_scal_z)
		* DirectX::XMMatrixRotationRollPitchYaw(this->m_rotation_x, this->m_rotation_y, this->m_rotation_z);
	return DirectX::XMMatrixTranspose(matrix * pro);
}