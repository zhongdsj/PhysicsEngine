# include <MyDx11/DrawAble/Rectangle2DDrawAble.h>
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

ZDSJ::Rectangle2DDrawAble::Rectangle2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context, const DrawAbleData& _data) : DrawAbleBase<Rectangle2DDrawAble>(_data)
{
	if (!this->isStaticInitialized()) {
		// 顶点缓存
		std::vector<ZDSJ::Vertex2D> vertices = {
			{-0.5f, 1.0f, 255, 0, 0, 255},
			{0.5f, 1.0f, 0, 255, 0, 128},
			{-0.5f, 0.0f, 0, 0, 255, 255},
			{0.5f, 0.0f, 255, 0, 0, 128},
		};
		// 顶点索引
		const UINT16 indices[] = {
			0, 1, 3,
			3, 2, 0
		};
		// 顶点缓存
		this->addStaticBind(new ZDSJ::VertexBufferBindAble(_device, vertices));
		// this->m_bind_able->push_back();
		// 顶点索引
		this->addStaticIndexBuffer(new ZDSJ::IndexBufferBindAble(_device, indices, sizeof(indices)), sizeof(indices) / sizeof(UINT16));
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
}

ZDSJ::Rectangle2DDrawAble::Rectangle2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context) : Rectangle2DDrawAble(_device, _context, DrawAbleData())
{

}

ZDSJ::Rectangle2DDrawAble::~Rectangle2DDrawAble()
{

}
