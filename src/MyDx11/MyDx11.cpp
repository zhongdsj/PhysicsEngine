# include <MyDx11/MyDx11.h>
# include <MyDx11/DrawAble/Triangle2DDrawAble.h>
# include <MyDx11/DrawAble/Rectangle2DDrawAble.h>
# include <MyDx11/DrawAble/Arc2DDrawAble.h>
# include <MyDx11/Animations/DrawAbleAnimation.h>
# include <MyDx11/DrawAbleManager.h>
# include <DirectXMath.h>
# include <d3d11.h>
# include <MyDx11/Context.h>
# include <sstream>
# include <Persistence.h>

ZDSJ::MyDx11::MyDx11(HWND _hwnd, int _window_width, int _window_height, ZDSJ::RenderType _render_type)
{
	// 创建设备及交换链
	DXGI_MODE_DESC buffer_desc;
	ZeroMemory(&buffer_desc, sizeof(DXGI_MODE_DESC));
	buffer_desc.Width = 0; // 这里设置窗口大小会导致imgui中viewPort与窗口大小不同，结果是右下角有黑边，且imgui显示有bug, 我不理解但我大为震撼
	buffer_desc.Height = 0;
	buffer_desc.RefreshRate.Numerator = 0;
	buffer_desc.RefreshRate.Denominator = 1;
	buffer_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	buffer_desc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	buffer_desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	ZeroMemory(&swap_chain_desc, sizeof(DXGI_SWAP_CHAIN_DESC));
	swap_chain_desc.BufferDesc = buffer_desc;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.BufferCount = 1;
	swap_chain_desc.OutputWindow = _hwnd;
	swap_chain_desc.Windowed = TRUE;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, NULL, nullptr, NULL, D3D11_SDK_VERSION, &swap_chain_desc,
	                              &this->m_swap_chain, &this->m_device, nullptr, &this->m_context);
	
	// 创建呈现目标
	ID3D11Texture2D* back_buffer;
	this->m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
	if (back_buffer != nullptr) {
		this->m_device->CreateRenderTargetView(back_buffer, nullptr, &this->m_render_target_view);
	}
	SAFE_RELEASE(back_buffer);
	// 绑定呈现目标
	this->m_context->OMSetRenderTargets(1, &this->m_render_target_view, nullptr);

	// 透明
	ID3D11BlendState* blend_state;
	D3D11_BLEND_DESC blend_desc = {};
	blend_desc.AlphaToCoverageEnable = FALSE;
	blend_desc.IndependentBlendEnable = FALSE;
	blend_desc.RenderTarget->BlendEnable = TRUE; // 是否开启混合
	blend_desc.RenderTarget->SrcBlend = D3D11_BLEND_SRC_ALPHA; // 将源图的 alpha 作为 src rgb 的混合因子
	blend_desc.RenderTarget->DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // 将源图的 1-alpha 作为 dst rgb 的混合因子
	blend_desc.RenderTarget->BlendOp = D3D11_BLEND_OP_ADD; // 进行相加操作
	blend_desc.RenderTarget->SrcBlendAlpha = D3D11_BLEND_ZERO; // 
	blend_desc.RenderTarget->DestBlendAlpha = D3D11_BLEND_ZERO;
	blend_desc.RenderTarget->BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend_desc.RenderTarget->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // 可以写入的位置
	this->m_device->CreateBlendState(&blend_desc, &blend_state);
	constexpr FLOAT blend_factor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	this->m_context->OMSetBlendState(blend_state, blend_factor, 0xffffffff);
	SAFE_RELEASE(blend_state);

	// 三角形
	this->m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// 将视区数组绑定到管道的光栅器阶段
	D3D11_VIEWPORT view_port;
	view_port.TopLeftX = 0;
	view_port.TopLeftY = 0;
	view_port.Width = _window_width;
	view_port.Height = _window_height;
	view_port.MinDepth = 0.0f;
	view_port.MaxDepth = 1.0f;
	this->m_context->RSSetViewports(1, &view_port);

	// 
	this->m_drawable_manager = new ZDSJ::DrawAbleManager(_render_type);

	ZDSJ::Context::getInstance()->keyboard()->registerKeyboard(ZDSJ::Key::ctrl, ZDSJ::Key::mouse_left, "add element in mouse position", [&](float _data){
		short x;
		short y;
		ZDSJ::Context::getInstance()->keyboard()->splitFloatToShorts(_data, x, y);
		const ZDSJ::Point word_pos = ZDSJ::Context::getInstance()->camera()->viewPosToWordPos(ZDSJ::Point(x, y));
		// const ZDSJ::Point word_pos = ZDSJ::Context::getInstance()->mouseWord();
		this->m_create(word_pos.x, word_pos.y, this->m_create_width, this->m_create_height);
	});

	ZDSJ::Context::getInstance()->command()->registerCommand("dx11", "create_size", "change create node size. dx11:create_size:width:height", [&](const std::string& _data) -> bool {
		std::ostringstream oss;
		if (_data.empty()) {
			oss << "dx11:create_size:(" << this->m_create_width << ", " << this->m_create_height << ")";
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
		try
		{
			width = std::stof(width_str.data());
			height = std::stof(height_str.data());
		}
		catch (std::out_of_range& ignore)
		{
			// TODO log
			oss << "dx11:createSize value to float out of range. data: " << width_str << "," << height_str;
			ZDSJ::Context::getInstance()->command()->write(oss.str());
		}
		catch (std::invalid_argument& ignore)
		{
			// TODO log
			oss << "dx11:createSize value to float invalid argument. data: " << width_str << "," << height_str;
			ZDSJ::Context::getInstance()->command()->write(oss.str());
		}
		
		if (width == 0.0f || height == 0.0f) {
			return false;
		}
		this->m_create_width = width;
		this->m_create_height = height;
		return true;
	});

	ZDSJ::Context::getInstance()->command()->registerCommand("dx11", "create", "change create function: [triangle2D, rectangle2D, circle2D]", [&](const std::string& _data) -> bool {
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

	ZDSJ::Context::getInstance()->command()->registerCommand("dx11", "save", "save now node to file. dx11:save:filename", [&](const std::string& _data)->bool {
		const auto persistence = new ZDSJ::Persistence(_data + ".nodes", true);
		if (!persistence->save(this->m_drawable_manager)) {
			ZDSJ::Context::getInstance()->command()->write("can not open file");
			return false;
		}
		delete persistence;
		ZDSJ::Context::getInstance()->command()->write("save to file: " + _data + ".nodes");
		return true;
	});

	ZDSJ::Context::getInstance()->command()->registerCommand("dx11", "load", "load node from file. dx11:save:filename", [&](const std::string& _data)->bool {
		ZDSJ::Context::getInstance()->wordActive(false);
		const auto persistence = new ZDSJ::Persistence(_data + ".nodes");
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
	ZDSJ::Context::getInstance()->mouseRight(false);
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
