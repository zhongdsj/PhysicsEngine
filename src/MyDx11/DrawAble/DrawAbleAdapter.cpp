# include <MyDx11/DrawAble/DrawAbleAdapter.h>
# include <MyDx11/BindAble/BindAbleInterface.h>
# include <MyDx11/BindAble/VertexConstantBufferBindAble.h>
# include <d3d11.h>
# include <DirectXMath.h>
# include <MyDx11/Context.h>
# include <MyDx11/VertexStructure.h>
# include <MyDx11/BindAble/IndexBufferBindAble.h>
# include <MyDx11/BindAble/VertexBufferBindAble.h>
# include <MyDx11/MyDx11.h>

ZDSJ::DrawAbleAdapter::DrawAbleAdapter(const DrawAbleData& _data) : m_bind_able(new std::vector<ZDSJ::BindAbleInterface*>), m_data(new DrawAbleData(_data))
{
	
}

void ZDSJ::DrawAbleAdapter::draw(ID3D11DeviceContext* _context, bool _bind_static)
{
	this->bind(_context);
	if (_bind_static) {
		this->bindStatic(_context);
	}
	this->update(_context);
	if (!this->hasState(ZDSJ::DrawAbleState::Check)) {
		this->drawIndex(_context);
	}
	if (this->hasState(ZDSJ::DrawAbleState::Hover) || this->hasState(ZDSJ::DrawAbleState::Check)) {
		this->drawBorder(_context);
	}
	
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

void ZDSJ::DrawAbleAdapter::update(ID3D11DeviceContext* _context)
{
	for (auto item : this->m_animation) {
		item->update(this, true);
	}
	this->m_transform->update(_context, this->getTransformMatix());
}

void ZDSJ::DrawAbleAdapter::bind(ID3D11DeviceContext* _context)
{
	for (auto item : *this->m_bind_able) {
		item->bind(_context);
	}
}

void ZDSJ::DrawAbleAdapter::bindStatic(ID3D11DeviceContext* _context)
{
	for (auto item : this->getStaticBindAble()) {
		item->bind(_context);
	}
	this->getColorVertex()->bind(_context);
}

void ZDSJ::DrawAbleAdapter::drawIndex(ID3D11DeviceContext* _context, unsigned int _start_index_location, int _base_vertex_location)
{
	_context->DrawIndexed(this->getStaticIndexSize(), _start_index_location, _base_vertex_location);
}

DirectX::XMMATRIX ZDSJ::DrawAbleAdapter::getTransformMatix() const
{
	// 世界矩阵->视图矩阵->投影矩阵
	// 世界矩阵
	DirectX::XMMATRIX size = DirectX::XMMatrixScaling(this->m_data->size.x, this->m_data->size.y, this->m_data->size.z);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(this->m_data->rotation.x, this->m_data->rotation.y, this->m_data->rotation.z);
	DirectX::XMMATRIX pos = DirectX::XMMatrixTranslation(this->m_data->pos.x, this->m_data->pos.y, this->m_data->pos.z);
	DirectX::XMMATRIX word = size * rotation * pos;
	
	DirectX::XMMATRIX matrix = word * ZDSJ::Context::getInstance()->camera()->getCarmeraMatrix();
	return DirectX::XMMatrixTranspose(matrix);
}

bool ZDSJ::DrawAbleAdapter::pointInPolgon2D(float _x, float _y)
{
	bool result = false;
	size_t triangle_size = this->m_indices.size() / 3;
	// 世界矩阵
	DirectX::XMMATRIX size = DirectX::XMMatrixScaling(this->m_data->size.x, this->m_data->size.y, this->m_data->size.z);
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYaw(this->m_data->rotation.x, this->m_data->rotation.y, this->m_data->rotation.z);
	DirectX::XMMATRIX pos = DirectX::XMMatrixTranslation(this->m_data->pos.x, this->m_data->pos.y, this->m_data->pos.z);
	DirectX::XMMATRIX word = size * rotation * pos;
	for (int i = 0; i < this->m_indices.size(); i += 3) {
		result = this->pointInTriangle2D(_x, _y, i, word);
		if (result) {
			break;
		}
	}
	if (result) {
		this->addState(ZDSJ::DrawAbleState::Hover);
	}
	else {
		this->removeState(ZDSJ::DrawAbleState::Hover);
	}
	return result;
}

void ZDSJ::DrawAbleAdapter::drawBorder(ID3D11DeviceContext* _context)
{
	// 准备绘制border资源
	ZDSJ::Context::getInstance()->dx11()->wireframe();
	this->getBorderVertex()->bind(_context);
	// 绘制
	this->drawIndex(_context);
	// 恢复原有资源
	ZDSJ::Context::getInstance()->dx11()->solid();
	this->getColorVertex()->bind(_context);
}

void ZDSJ::DrawAbleAdapter::setVertexBufferAndIndexBuffer(ID3D11Device* _device, ID3D11DeviceContext* _context, const ZDSJ::VertexBufferBindAble*& _vertex_buffer_bindable, const ZDSJ::IndexBufferBindAble*& _index_buffer_bindable)
{
	ZDSJ::Vertex2D* vertex_buffer = nullptr;
	size_t vertex_buffer_size = 0;
	_vertex_buffer_bindable->getData(_device, _context, reinterpret_cast<void**>(&vertex_buffer), vertex_buffer_size);
	this->m_vertices.reserve(vertex_buffer_size / sizeof(ZDSJ::Vertex2D));
	this->m_vertices.assign(&vertex_buffer[0], &vertex_buffer[vertex_buffer_size / sizeof(ZDSJ::Vertex2D)]);
	delete[] vertex_buffer;

	// 顶点缓存
	UINT16* index_buffer = nullptr;
	size_t index_buffer_size = 0;
	_index_buffer_bindable->getData(_device, _context, reinterpret_cast<void**>(&index_buffer), index_buffer_size);
	this->m_indices.reserve(index_buffer_size / sizeof(UINT16));
	this->m_indices.assign(&index_buffer[0], &index_buffer[index_buffer_size / sizeof(UINT16)]);
	delete[] index_buffer;
}

bool ZDSJ::DrawAbleAdapter::pointInTriangle2D(float _x, float _y, short _triangle_index, DirectX::XMMATRIX& _word_matrix)
{
	DirectX::XMVECTOR x_0 = DirectX::XMVectorSet(this->m_vertices.at(this->m_indices[_triangle_index]).pos.x, this->m_vertices.at(this->m_indices[_triangle_index]).pos.y, 1, 1);
	DirectX::XMVECTOR x_1 = DirectX::XMVectorSet(this->m_vertices.at(this->m_indices[_triangle_index + 1]).pos.x, this->m_vertices.at(this->m_indices[_triangle_index + 1]).pos.y, 1, 1);
	DirectX::XMVECTOR x_2 = DirectX::XMVectorSet(this->m_vertices.at(this->m_indices[_triangle_index + 2]).pos.x, this->m_vertices.at(this->m_indices[_triangle_index + 2]).pos.y, 1, 1);
	x_0 = DirectX::XMVector4Transform(x_0, _word_matrix);
	x_1 = DirectX::XMVector4Transform(x_1, _word_matrix);
	x_2 = DirectX::XMVector4Transform(x_2, _word_matrix);
	
	DirectX::XMVECTOR x_10 = DirectX::XMVectorSubtract(x_1, x_0);
	DirectX::XMVECTOR x_20 = DirectX::XMVectorSubtract(x_2, x_0);
	DirectX::XMVECTOR normal = DirectX::XMVector3Cross(x_10, x_20);
	// normal = DirectX::XMVector2Normalize(normal); 这里不需要归一化，因为我们只关注符号
	DirectX::XMVECTOR position = DirectX::XMVectorSet(_x, _y, 1, 1);
	// (x0-p)x(x2-p)*n;
	DirectX::XMVECTOR r_0 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(position, x_0), x_20);
	DirectX::XMVECTOR r_1 = DirectX::XMVector3Cross(x_10, DirectX::XMVectorSubtract(position, x_0));
	DirectX::XMVECTOR r_2 = DirectX::XMVector3Cross(DirectX::XMVectorSubtract(position, x_1), DirectX::XMVectorSubtract(position, x_2));
	float d_0 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(r_0, normal));
	float d_1 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(r_1, normal));
	float d_2 = DirectX::XMVectorGetX(DirectX::XMVector3Dot(r_2, normal));
	bool isInside = (d_0 >= 0.0f && d_1 >= 0.0f && d_2 >= 0.0f);
	return isInside;
}
