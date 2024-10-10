# include <MyDx11/DrawAble/DrawAbleAdapter.h>
# include <MyDx11/BindAble/BindAbleInterface.h>
# include <MyDx11/BindAble/VertexConstantBufferBindAble.h>
# include <d3d11.h>
# include <DirectXMath.h>

ZDSJ::DrawAbleAdapter::DrawAbleAdapter(ID3D11Device* _device, const DrawAbleData& _data) : m_bind_able(new std::vector<ZDSJ::BindAbleInterface*>), m_data(new DrawAbleData(_data))
{
}

void ZDSJ::DrawAbleAdapter::draw(ID3D11DeviceContext* _context, float _fps)
{
	this->bind(_context);
	this->update(_context, _fps);
	_context->DrawIndexed(this->m_index_size, 0u, 0u);
}

const ZDSJ::DrawAbleData* ZDSJ::DrawAbleAdapter::getData() const
{
	return this->m_data;
}

ZDSJ::DrawAbleAdapter::~DrawAbleAdapter()
{
	delete this->m_bind_able;
	delete this->m_data;
	for (auto item : this->m_animation) {
		delete item;
	}
}

void ZDSJ::DrawAbleAdapter::update(ID3D11DeviceContext* _context, float _fps)
{
	for (auto item : this->m_animation) {
		item->update(this, _fps, true);
	}
	this->m_transform->update(_context, this->getTransformMatix());
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
	DirectX::XMMATRIX matrix = DirectX::XMMatrixTranslation(this->m_data->translation.x, this->m_data->translation.y, this->m_data->translation.z)
		* DirectX::XMMatrixScaling(this->m_data->scale.x, this->m_data->scale.y, this->m_data->scale.z)
		* DirectX::XMMatrixRotationRollPitchYaw(this->m_data->rotation.x, this->m_data->rotation.y, this->m_data->rotation.z);
	return DirectX::XMMatrixTranspose(matrix * pro);
	// return DirectX::XMMatrixTranspose(matrix);
}