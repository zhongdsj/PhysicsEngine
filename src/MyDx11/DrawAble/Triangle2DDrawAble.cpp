# include <MyDx11/DrawAble/Triangle2DDrawAble.h>
# include <MyDx11/BindAble/VertexBufferBindAble.h>
# include <MyDx11/BindAble/IndexBufferBindAble.h>
# include <MyDx11/BindAble/VertexShaderBindAble.h>
# include <MyDx11/BindAble/PixelShaderBindAble.h>
# include <MyDx11/BindAble/InputLayoutBindAble.h>
# include <MyDx11/VertexStructure.h>
# include <MyDx11/BindAble/VertexConstantBufferBindAble.h>
# include <MyDx11/Shader/TriangleVertexShader2D.h>
# include <MyDx11/Shader/TrianglePixelShader2D.h>

# include <d3d11.h>
# include <iterator>


ZDSJ::Triangle2DDrawAble::Triangle2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context, float _translation_x, bool _change) : m_change(_change), DrawAbleAdapter(_device) {
	// 顶点缓存
	std::vector<ZDSJ::Vertex2D> vertices = {
		{0.0f, 1.0f, 255, 0, 0, 128},
		{0.5f, 0.0f, 0, 255, 0, 255},
		{-0.5f, 0.0f, 0, 0, 255, 128},
	};
	// 顶点索引
	const UINT16 indices[] = {
		0,1,2
	};
	// 顶点缓存
	this->m_bind_able->push_back(new ZDSJ::VertexBufferBindAble(_device, vertices));
	// 顶点索引
	this->m_bind_able->push_back(new ZDSJ::IndexBufferBindAble(_device, indices, sizeof(indices)));
	this->m_index_size = sizeof(indices) / sizeof(UINT16);
	// 顶点着色器
	this->m_bind_able->push_back(new ZDSJ::VertexShaderBindAble(_device, g_main_vertex_shader, sizeof(g_main_vertex_shader)));
	// layout
	std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
		{"Position", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	this->m_bind_able->push_back(new ZDSJ::InputLayoutBindAble(_device, ied, g_main_vertex_shader, sizeof(g_main_vertex_shader)));
	// 像素着色器
	this->m_bind_able->push_back(new ZDSJ::PixelShaderBindAble(_device, g_main_pixel_shader, sizeof(g_main_pixel_shader)));
	// 缩放旋转矩阵
	this->m_transform = new ZDSJ::VertexConstantBufferBindAble(_device);
	this->m_bind_able->push_back(m_transform);
	this->m_translation_x = _translation_x;
	// this->m_change = _change;
	// this->bind(_context);
}

void ZDSJ::Triangle2DDrawAble::addAnimation(ZDSJ::DrawAbleAnimation* _animation)
{
	this->m_animation.push_back(_animation);
}

ZDSJ::DrawAbleAnimation* ZDSJ::Triangle2DDrawAble::rotationZAnimation(float _change_value, long long _amination_time, short _fps, bool _loop)
{
	return new ZDSJ::DrawAbleAnimation((_change_value / 180.0f) * DirectX::XM_PI, _amination_time, _fps, [&](float _value) {
		this->setRotationZ(_value);
		}, [](float _step, float _change)->float{
			return _step * _change;
		});
}

void ZDSJ::Triangle2DDrawAble::setRotationZ(float _value)
{
	this->m_rotation_z = _value;
}

ZDSJ::Triangle2DDrawAble::~Triangle2DDrawAble()
{
	for (auto item : this->m_animation) {
		delete item;
	}
}


void ZDSJ::Triangle2DDrawAble::update(ID3D11DeviceContext* _context, short _fps) {
	for (auto item : this->m_animation) {
		item->update(_fps, true);
	}
	/*if (this->m_change) {
		this->m_rotation_z += 2.0f * DirectX::XM_PI * 1 / this->m_max_fps;
	}*/
	
	// ++this->m_run_fps;
	// this->m_scal_x = this->m_scal_y = this->m_scal_z = 0.3f;
	if (this->m_run_fps == this->m_max_fps) {
		this->m_run_fps = 0;
	}
	this->m_transform->update(_context, this->getTransformMatix());
}