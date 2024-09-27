# include <MyDx11/Triangle2DDrawAble.h>
# include <MyDx11/VertexBufferBindAble.h>
# include <MyDx11/IndexBufferBindAble.h>
# include <MyDx11/VertexShaderBindAble.h>
# include <MyDx11/PixelShaderBindAble.h>
# include <MyDx11/InputLayoutBindAble.h>
# include <MyDx11/VertexStructure.h>
# include <MyDx11/VertexConstantBufferBindAble.h>
# include <MyDx11/TriangleVertexShader2D.h>
# include <MyDx11/TrianglePixelShader2D.h>

# include <d3d11.h>
# include <iterator>


ZDSJ::Triangle2DDrawAble::Triangle2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context) : DrawAbleAdapter(_device) {
	// 顶点缓存
	std::vector<ZDSJ::Vertex2D> vertices = {
		{0.0f, 0.5f + 0.5f, 255, 0, 0, 128},
		{0.5f, -0.5f + 0.5f, 0, 255, 0, 255},
		{-0.5f, -0.5f + 0.5f, 0, 0, 255, 128},
	};
	// 顶点索引
	const UINT16 indices[] = {
		0,1,2
	};
	// 顶点缓存
	this->m_bind_able->push_back(new ZDSJ::VertexBufferBindAble(_device, vertices));
	// 缩放矩阵
	/*const ZDSJ::VertexScal2D vertex_scal = { 1.0f, 1.0f, 0.0f, 0.0f };
	this->m_bind_able->push_back(new ZDSJ::VertexScalBuffer2DBindAble(_device, &vertex_scal));*/
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
	// 
	this->m_transform = new ZDSJ::VertexConstantBufferBindAble(_device);
	this->m_bind_able->push_back(m_transform);
	// DirectX::XMMatrixRotationRollPitchYaw(); // 旋转(欧拉角)
	// DirectX::XMMatrixTranslation(); // 平移
	this->bind(_context);
	// DirectX::XMMatrixMultiply();
	/*ZDSJ::VertexConstantBuffer vertex_constant_buffer = { DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f), DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f), DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 1.0f) };
	DirectX::XMMATRIX matrix = DirectX::XMMatrixMultiply(vertex_constant_buffer.scal, vertex_constant_buffer.rotation);
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(matrix);
	cbd.StructureByteStride = 0u;

	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &matrix;
	_device->CreateBuffer(&cbd, &csd, &this->m_buffer);
	_context->VSSetConstantBuffers(0u, 1u, &this->m_buffer);*/
}


void ZDSJ::Triangle2DDrawAble::update(ID3D11DeviceContext* _context, short _fps) {
	this->m_rotation_z = 2.0f * DirectX::XM_PI * this->m_run_fps / this->m_max_fps;
	++this->m_run_fps;
	if (this->m_run_fps == this->m_max_fps) {
		this->m_run_fps = 0;
	}
	// this->m_scal_x = 3.0f / 4;
	this->m_transform->update(_context, this->getTransformMatix());
	// D3D11_MAPPED_SUBRESOURCE msr;
	// _context->Map()
	
}