# include <MyDx11/DrawAble/DrawAbleAdapter.h>
# include <MyDx11/BindAble/BindAbleInterface.h>
# include <MyDx11/BindAble/VertexConstantBufferBindAble.h>
# include <d3d11.h>
# include <DirectXMath.h>
# include <MyDx11/VertexStructure.h>
# include <MyDx11/BindAble/IndexBufferBindAble.h>
# include <MyDx11/BindAble/VertexBufferBindAble.h>
# include <MyDx11/Camera.h>
# include <Context.h>
# include <Physics/MovementInterface.h>
# include <MyDx11/DrawAbleData.h>

ZDSJ::DrawAbleAdapter::DrawAbleAdapter() : m_bind_able(new std::vector<std::shared_ptr<BindAbleInterface>>)
{
	
}

void ZDSJ::DrawAbleAdapter::draw(ID3D11DeviceContext* _context, const DrawAbleData* _drawAbleData, bool _bind_static)
{
	this->bind(_context);
	if (_bind_static) {
		this->bindStatic(_context);
	}
	this->update(_context, _drawAbleData);
	if (!this->hasState(ZDSJ::DrawAbleState::Check)) {
		this->drawIndex(_context);
	}
	if (this->hasState(ZDSJ::DrawAbleState::Hover) || this->hasState(ZDSJ::DrawAbleState::Check)) {
		this->drawBorder(_context);
	}
	
}

ZDSJ::DrawAbleAdapter::~DrawAbleAdapter()
{
	delete this->m_bind_able;
}

void ZDSJ::DrawAbleAdapter::update(ID3D11DeviceContext* _context, const DrawAbleData* _drawAbleData)
{
	this->m_transform.get()->update(_context, this->getTransformMatix(_drawAbleData));
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

DirectX::XMMATRIX ZDSJ::DrawAbleAdapter::getTransformMatix(const DrawAbleData* _drawAbleData) const
{
	// 世界矩阵->视图矩阵->投影矩阵
	// 世界矩阵
	DirectX::XMMATRIX size = DirectX::XMMatrixScalingFromVector(DirectX::XMLoadFloat3(&_drawAbleData->renderSize()));
	DirectX::XMMATRIX rotation = DirectX::XMMatrixRotationRollPitchYawFromVector(DirectX::XMLoadFloat3(&_drawAbleData->renderRotation()));
	DirectX::XMMATRIX position = DirectX::XMMatrixTranslationFromVector(DirectX::XMLoadFloat3(&_drawAbleData->renderPosition()));

	DirectX::XMMATRIX word = size * rotation * position;
	
	DirectX::XMMATRIX matrix = word * Camera_Instance->getCameraMatrix();

	//// TODO
	//// XMVector3Project方法
	//
	//DirectX::XMVECTOR v = DirectX::XMVectorSet(20.0f, 20.0f, 0.0f, 1.0f);
	//// BoundingFrustum创建视锥
	//DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(60.0f, 800.0f/600.0f, 0.1f, 1000.0f);
	//DirectX::BoundingFrustum frustum(projection);
	//// 1. 定义相机位置、目标点和上方向
	//DirectX::XMVECTOR eyePosition = DirectX::XMVectorSet(0.0f, 0.0f, -200.0f, 0.0f);
	//DirectX::XMVECTOR focusPosition = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	//DirectX::XMVECTOR upDirection = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	//// 2. 创建视图矩阵
	//DirectX::XMMATRIX viewMatrix = DirectX::XMMatrixLookAtLH(eyePosition, focusPosition, upDirection);
	//// frustum.Transform(frustum, XMMatrixInverse(nullptr, viewMatrix));
	//auto result = DirectX::XMVector3Project(v, 0, 0, 800, 600, 0.0f, 1.0f, projection, viewMatrix, word);
	return DirectX::XMMatrixTranspose(matrix);
}

void ZDSJ::DrawAbleAdapter::drawBorder(ID3D11DeviceContext* _context)
{
	// 准备绘制border资源
	ZDSJ::Context::getInstance()->getDx11()->wireframe();
	this->getBorderVertex()->bind(_context);
	// 绘制
	this->drawIndex(_context);
	// 恢复原有资源
	ZDSJ::Context::getInstance()->getDx11()->solid();
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

bool ZDSJ::DrawAbleAdapter::pointInTriangle2D(float _x, float _y, short _triangle_index, DirectX::XMMATRIX& _word_matrix) const
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
	bool is_inside = (d_0 >= 0.0f && d_1 >= 0.0f && d_2 >= 0.0f);
	return is_inside;
}

bool ZDSJ::DrawAbleAdapter::rayInTriangle2D(float _x, float _y, short _triangle_index, DirectX::XMMATRIX& _word_matrix) const
{
	// M-T算法
	DirectX::XMVECTOR x_0 = DirectX::XMVectorSet(this->m_vertices.at(this->m_indices[_triangle_index]).pos.x, this->m_vertices.at(this->m_indices[_triangle_index]).pos.y, 1, 1);
	DirectX::XMVECTOR x_1 = DirectX::XMVectorSet(this->m_vertices.at(this->m_indices[_triangle_index + 1]).pos.x, this->m_vertices.at(this->m_indices[_triangle_index + 1]).pos.y, 1, 1);
	DirectX::XMVECTOR x_2 = DirectX::XMVectorSet(this->m_vertices.at(this->m_indices[_triangle_index + 2]).pos.x, this->m_vertices.at(this->m_indices[_triangle_index + 2]).pos.y, 1, 1);
	x_0 = DirectX::XMVector4Transform(x_0, _word_matrix);
	x_1 = DirectX::XMVector4Transform(x_1, _word_matrix);
	x_2 = DirectX::XMVector4Transform(x_2, _word_matrix);
	DirectX::XMVECTOR x_10 = DirectX::XMVectorSubtract(x_1, x_0);
	DirectX::XMVECTOR x_20 = DirectX::XMVectorSubtract(x_2, x_0);
	auto camera_pos = Camera_Instance->cameraPos();
	DirectX::XMVECTOR origin = { camera_pos.x, camera_pos.y, camera_pos.z, camera_pos.w };
	DirectX::XMFLOAT3 direction = { _x - camera_pos.x, _y - camera_pos.y, -camera_pos.z };
	DirectX::XMVECTOR dir_vec = DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&direction));
	DirectX::XMStoreFloat3(&direction, dir_vec);
	// 计算向量叉乘
	DirectX::XMVECTOR p = DirectX::XMVector3Cross(DirectX::XMLoadFloat3(&direction), x_20);
	float det = DirectX::XMVectorGetX(DirectX::XMVector3Dot(x_10, p));
	if(det == 0)
	{
		return false;
	}
	float inv_det = 1.0f / det;
	
	DirectX::XMVECTOR t = DirectX::XMVectorSubtract(origin, x_0);
	float u = DirectX::XMVectorGetX(DirectX::XMVector3Dot(t, p)) * inv_det;
	if (u < 0.0f || u > 1.0f)
	{
		return false;
	}

	DirectX::XMVECTOR q = DirectX::XMVector3Cross(t, x_10);
	float v = DirectX::XMVectorGetX(DirectX::XMVector3Dot(DirectX::XMLoadFloat3(&direction), q)) * inv_det;
	if (v < 0.0f || u + v > 1.0f)
	{
		return false;
	}

	// 计算 t 值（射线参数）
	float t_ = DirectX::XMVectorGetX(DirectX::XMVector3Dot(x_20, q)) * inv_det;

	// 如果 t 是负的，表示交点在射线起点之后（即不在射线上），因此不相交
	if (t_ < 0.0f)
	{
		return false;
	}
	auto d = DirectX::XMLoadFloat3(&direction);
	auto d_x = DirectX::XMVectorGetX(d);
	auto d_y = DirectX::XMVectorGetY(d);
	auto d_z = DirectX::XMVectorGetZ(d);
	DirectX::XMVectorSetZ(d, 1.0f);
	auto intersection = DirectX::XMVectorAdd(origin, DirectX::XMVectorScale(DirectX::XMLoadFloat3(&direction), t_));
	// 计算交点
	return true;
}
