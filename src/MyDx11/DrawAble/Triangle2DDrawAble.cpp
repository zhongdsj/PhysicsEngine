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


ZDSJ::Triangle2DDrawAble::Triangle2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context, const DrawAbleData& _data) : DrawAbleBase<Triangle2DDrawAble>(_data) {
	if (!this->isStaticInitialized()) {
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
		this->addStaticBind(new ZDSJ::VertexBufferBindAble(_device, vertices));
		// 顶点索引
		this->addStaticIndexBuffer(new ZDSJ::IndexBufferBindAble(_device, indices, sizeof(indices)), sizeof(indices) / sizeof(UINT16));
		// this->m_index_size = ;
		// 顶点着色器
		this->addStaticBind(new ZDSJ::VertexShaderBindAble(_device, g_main_vertex_shader, sizeof(g_main_vertex_shader)));
		// layout
		std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
			{"Position", 0, DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
			{"Color", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 8u, D3D11_INPUT_PER_VERTEX_DATA, 0},
		};
		this->addStaticBind(new ZDSJ::InputLayoutBindAble(_device, ied, g_main_vertex_shader, sizeof(g_main_vertex_shader)));
		// 像素着色器
		this->addStaticBind(new ZDSJ::PixelShaderBindAble(_device, g_main_pixel_shader, sizeof(g_main_pixel_shader)));
	}
	// 缩放旋转矩阵
	this->m_transform = new ZDSJ::VertexConstantBufferBindAble(_device);
	this->m_bind_able->push_back(m_transform);
	// this->m_data->translation.x = _translation_x;
	// this->bind(_context);
}

ZDSJ::Triangle2DDrawAble::Triangle2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context) : Triangle2DDrawAble(_device, _context, DrawAbleData())
{
	
}

ZDSJ::Triangle2DDrawAble::Triangle2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context, float3 _size, float3 _pos) : Triangle2DDrawAble(_device, _context, DrawAbleData(_size, _pos, {0.0f, 0.0f, 0.0f}))
{
}

ZDSJ::Triangle2DDrawAble::~Triangle2DDrawAble()
{

}