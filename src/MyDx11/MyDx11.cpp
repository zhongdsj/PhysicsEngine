# include <MyDx11/MyDx11.h>
# include <MyDx11/DrawAble/Triangle2DDrawAble.h>
# include <MyDx11/DrawAble/Rectangle2DDrawAble.h>
# include <MyDx11/DrawAble/Arc2DDrawAble.h>
# include <MyDx11/Animations/DrawAbleAnimation.h>
# include <MyDx11/DrawAbleManager.h>
# include <DirectXMath.h>
# include <d3d11.h>
# include <MyDx11/Context.h>
# include <MyDx11/VertexStructure.h>
# include <sstream>
# include <Persistence.h>

ZDSJ::MyDx11::MyDx11(HWND _hwnd, int _window_width, int _window_height, ZDSJ::RenderType _render_type)
{
	// 创建设备及交换链
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width = 0; // 这里设置窗口大小会导致imgui中viewPort与窗口大小不同，结果是右下角有黑边，且imgui显示有bug, 我不理解但我大为震撼
	bufferDesc.Height = 0;
	bufferDesc.RefreshRate.Numerator = 0;
	bufferDesc.RefreshRate.Denominator = 1;
	bufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	bufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	bufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swapChainDesc.BufferDesc = bufferDesc;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = _hwnd;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &swapChainDesc,
		&this->m_swap_chain, &this->m_device, NULL, &this->m_context);
	
	// 创建呈现目标
	ID3D11Texture2D* backBuffer;
	this->m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (backBuffer != nullptr) {
		this->m_device->CreateRenderTargetView(backBuffer, nullptr, &this->m_render_target_view);
	}
	SAFE_RELEASE(backBuffer);
	// 绑定呈现目标
	this->m_context->OMSetRenderTargets(1, &this->m_render_target_view, nullptr);

	// 透明
	ID3D11BlendState* blendState;
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget->BlendEnable = TRUE; // 是否开启混合
	blendDesc.RenderTarget->SrcBlend = D3D11_BLEND_SRC_ALPHA; // 将源图的 alpha 作为 src rgb 的混合因子
	blendDesc.RenderTarget->DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // 将源图的 1-alpha 作为 dst rgb 的混合因子
	blendDesc.RenderTarget->BlendOp = D3D11_BLEND_OP_ADD; // 进行相加操作
	blendDesc.RenderTarget->SrcBlendAlpha = D3D11_BLEND_ZERO; // 
	blendDesc.RenderTarget->DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget->BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 可以写入的位置
	this->m_device->CreateBlendState(&blendDesc, &blendState);
	const FLOAT BlendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	this->m_context->OMSetBlendState(blendState, BlendFactor, 0xffffffff);
	SAFE_RELEASE(blendState);

	// 三角形
	this->m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 将视区数组绑定到管道的光栅器阶段
	D3D11_VIEWPORT viewPort;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = _window_width;
	viewPort.Height = _window_height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	this->m_context->RSSetViewports(1, &viewPort);

	// 
	this->m_drawable_manager = new ZDSJ::DrawAbleManager(_render_type);

	ZDSJ::Context::getInstance()->keyboard()->registeKeyboard(ZDSJ::Key::ctrl, ZDSJ::Key::mouse_left, "add element in mouse position", [&](float _data){
		short x;
		short y;
		ZDSJ::Context::getInstance()->keyboard()->splitFloatToShorts(_data, x, y);
		ZDSJ::Point word_pos = ZDSJ::Context::getInstance()->camera()->viewPosToWordPos(ZDSJ::Point(x, y));
		this->m_create(word_pos.x, word_pos.y, this->m_create_width, this->m_create_height);
	});

	ZDSJ::Context::getInstance()->command()->registeCommand("dx11", "createSize", "change create node size. dx11:createSize:width:height", [&](std::string& _data) -> bool {
		if (_data.empty()) {
			std::ostringstream oss;
			oss << "dx11:createSize:(" << this->m_create_width << ", " << this->m_create_height << ")";
			ZDSJ::Context::getInstance()->command()->write(oss.str());
			return true;
		}
		std::istringstream iss(_data);
		std::string width_str;
		std::string height_str;
		std::getline(iss, width_str, ':');
		std::getline(iss, height_str, ':');
		if (width_str.empty() || height_str.empty()) {
			return false;
		}
		float width = 0.0f;
		float height = 0.0f;
		width = std::atof(width_str.data());
		height = std::atof(height_str.data());
		if (width == 0.0f || height == 0.0f) {
			return false;
		}
		this->m_create_width = width;
		this->m_create_height = height;
		return true;
	});

	ZDSJ::Context::getInstance()->command()->registeCommand("dx11", "create", "change create function: [triangle2D, rectangle2D, circle2D]", [&](std::string& _data) -> bool {
		if (_data == "triangle2D") {
			this->m_create = std::bind(&MyDx11::createTriangle2D, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
			return true;
		}
		if (_data == "rectangle2D") {
			this->m_create = std::bind(&MyDx11::createRectangle2D, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
			return true;
		}
		if (_data == "circle2D") {
			this->m_create = std::bind(&MyDx11::createCircle2D, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);
			return true;
		}
		return false;
	});

	ZDSJ::Context::getInstance()->command()->registeCommand("dx11", "save", "save now node to file. dx11:save:filename", [&](std::string& _data)->bool {
		ZDSJ::Persistence* persistence = new ZDSJ::Persistence(_data, true);
		if (!persistence->save(this->m_drawable_manager)) {
			ZDSJ::Context::getInstance()->command()->write("can not open file");
			return false;
		}
		delete persistence;
		ZDSJ::Context::getInstance()->command()->write("save to file: " + _data + ".nodes");
		return true;
	});

	ZDSJ::Context::getInstance()->command()->registeCommand("dx11", "load", "load node from file. dx11:save:filename", [&](std::string& _data)->bool {
		ZDSJ::Context::getInstance()->wordActive(false);
		ZDSJ::Persistence* persistence = new ZDSJ::Persistence(_data);
		ZDSJ::Context::getInstance()->command()->write("load from file: " + _data + ".nodes");
		if (!persistence->load(this->m_device, this->m_context, this->m_drawable_manager)) {
			ZDSJ::Context::getInstance()->command()->write("can not open file");
			return false;
		}
		delete persistence;
		return true;
	});

	D3D11_RASTERIZER_DESC solid_desc;
	ZeroMemory(&solid_desc, sizeof(solid_desc));
	solid_desc.FillMode = D3D11_FILL_SOLID;
	solid_desc.CullMode = D3D11_CULL_BACK;
	solid_desc.ScissorEnable = false;
	solid_desc.DepthClipEnable = true;
	this->m_device->CreateRasterizerState(&solid_desc, &this->m_solid_rasterizer_state);

	D3D11_RASTERIZER_DESC wireframe_desc;
	ZeroMemory(&wireframe_desc, sizeof(wireframe_desc));
	wireframe_desc.FillMode = D3D11_FILL_WIREFRAME;
	wireframe_desc.CullMode = D3D11_CULL_BACK;
	wireframe_desc.ScissorEnable = false;
	wireframe_desc.DepthClipEnable = true;
	this->m_device->CreateRasterizerState(&wireframe_desc, &this->m_wireframe_rasterizer_state);
	
	this->solid();

	ZDSJ::Context::getInstance()->dx11(this);

	// this->m_drawable_manager->add((new Arc2DDrawAble(this->m_device, this->m_context, 20, 360))->setPosX(100));
}

void ZDSJ::MyDx11::render()
{
	this->clearByBackground();
	this->m_drawable_manager->render(m_context);
}

void ZDSJ::MyDx11::endRender()
{
	this->m_swap_chain->Present(0, 0);
	ZDSJ::Context::getInstance()->mouseClick(false);
}

void ZDSJ::MyDx11::solid()
{
	this->m_context->RSSetState(this->m_solid_rasterizer_state);
}

void ZDSJ::MyDx11::wireframe()
{
	this->m_context->RSSetState(this->m_wireframe_rasterizer_state);
}

ID3D11Device* ZDSJ::MyDx11::device() const
{
	return this->m_device;
}

ID3D11DeviceContext* ZDSJ::MyDx11::context() const
{
	return this->m_context;
}

ZDSJ::MyDx11::~MyDx11()
{
	delete this->m_drawable_manager;
	SAFE_RELEASE(this->m_render_target_view);
	SAFE_RELEASE(this->m_context);
	SAFE_RELEASE(this->m_swap_chain);
	SAFE_RELEASE(this->m_solid_rasterizer_state);
	SAFE_RELEASE(this->m_wireframe_rasterizer_state);
	SAFE_RELEASE(this->m_device);
}

void ZDSJ::MyDx11::clearByBackground()
{
	this->m_context->ClearRenderTargetView(this->m_render_target_view, this->m_background);
}

void ZDSJ::MyDx11::createTriangle2D(float _x, float _y, float _width, float _height)
{
	this->m_drawable_manager->add((new ZDSJ::Triangle2DDrawAble(this->m_device, this->m_context))
		->setPosX(_x)->setPosY(_y)->setSizeX(_width)->setSizeY(_height));
}

void ZDSJ::MyDx11::createRectangle2D(float _x, float _y, float _width, float _height)
{
	this->m_drawable_manager->add((new ZDSJ::Rectangle2DDrawAble(this->m_device, this->m_context))
	->setPosX(_x)->setPosY(_y)->setSizeX(_width)->setSizeY(_height));
}

void ZDSJ::MyDx11::createCircle2D(float _x, float _y, float _width, float _height)
{
	this->m_drawable_manager->add((new ZDSJ::Arc2DDrawAble(this->m_device, this->m_context, 20, 360))
		->setPosX(_x)->setPosY(_y)->setSizeX(_width)->setSizeY(_height));
}
