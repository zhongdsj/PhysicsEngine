# include <MyDx11/DrawAble/Arc2DDrawAble.h>
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

ZDSJ::Arc2DDrawAble::Arc2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context, int _divisions, float _rotate, const DrawAbleData& _data) : DrawAbleBase<Arc2DDrawAble>(_data)
{
	if (!this->isStaticInitialized()) {
		float r = 0.5f;
		// 顶点缓存
		std::vector<ZDSJ::Vertex2D> vertices = {
			{ 0.0f, 0.0f, 22, 177, 224, 255 },
		};
		_rotate = DirectX::XMConvertToRadians(_rotate);
		float angle_step = _rotate / _divisions;
		float angle = 0.0f;
		for (int i = 1; i <= _divisions; ++i) {
			angle = i * angle_step;
			vertices.push_back({ r * DirectX::XMScalarCos(angle), r * DirectX::XMScalarSin(angle), 22, 177, 224, 255 });
			/*if (i % 3 == 0) {
				vertices.push_back({ r * DirectX::XMScalarCos(angle), r * DirectX::XMScalarSin(angle), 255, 0, 0, 128});
			}
			else if (i % 3 == 1) {
				vertices.push_back({ r * DirectX::XMScalarCos(angle), r * DirectX::XMScalarSin(angle), 0, 255, 0, 255 });
			}
			else if (i % 3 == 2) {
				vertices.push_back({ r * DirectX::XMScalarCos(angle), r * DirectX::XMScalarSin(angle), 0, 0, 255, 128 });
			}*/
		}
		std::vector<UINT16> indices;
		for (int i = 1; i < _divisions; ++i) {
			indices.push_back(i + 1);
			indices.push_back(i);
			indices.push_back(0);
		}
		indices.push_back(1);
		indices.push_back(_divisions);
		indices.push_back(0);
		// 顶点缓存
		// this->addStaticBind(new ZDSJ::VertexBufferBindAble(_device, vertices));
		this->setVertexBindAble(_device, _context, new ZDSJ::VertexBufferBindAble(_device, vertices));
		// 顶点索引
		this->addStaticIndexBuffer(new ZDSJ::IndexBufferBindAble(_device, indices.data(), indices.size() * sizeof(UINT16)), indices.size());
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
	const ZDSJ::VertexBufferBindAble* vertex_buffer_bindable = nullptr;
	const ZDSJ::IndexBufferBindAble* index_buffer_bindable = nullptr;
	if (this->getStaticVertexBufferAndIndexBuffer(vertex_buffer_bindable, index_buffer_bindable)) {
		this->setVertexBufferAndIndexBuffer(_device, _context, vertex_buffer_bindable, index_buffer_bindable);
	}
	// 缩放旋转矩阵
	this->m_transform = new ZDSJ::VertexConstantBufferBindAble(_device);
	this->m_bind_able->push_back(m_transform);
}

ZDSJ::Arc2DDrawAble::Arc2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context, int _divisions, float _rotate)
	:Arc2DDrawAble(_device, _context, _divisions, _rotate, DrawAbleData())
{
}

ZDSJ::Arc2DDrawAble::Arc2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context, int _divisions, float _rotate, float3 _size, float3 _pos)
	:Arc2DDrawAble(_device, _context, _divisions, _rotate, DrawAbleData(_size, _pos, { 0.0f, 0.0f, 0.0f }))
{
}

ZDSJ::Arc2DDrawAble::~Arc2DDrawAble()
{
}
