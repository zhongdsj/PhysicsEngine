# include <MyDx11/MyDx11.h>
# include <MyDx11/DrawAble/Triangle2DDrawAble.h>
# include <MyDx11/DrawAble/Rectangle2DDrawAble.h>
# include <MyDx11/Animations/DrawAbleAnimation.h>
# include <DirectXMath.h>
# include <d3d11.h>

ZDSJ::MyDx11::MyDx11(HWND _hwnd, int _window_width, int _window_height)
{
	// �����豸��������
	DXGI_MODE_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(DXGI_MODE_DESC));
	bufferDesc.Width = 0; // �������ô��ڴ�С�ᵼ��imgui��viewPort�봰�ڴ�С��ͬ����������½��кڱߣ���imgui��ʾ��bug, �Ҳ���⵫�Ҵ�Ϊ��
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
	
	// ��������Ŀ��
	ID3D11Texture2D* backBuffer;
	this->m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	if (backBuffer != nullptr) {
		this->m_device->CreateRenderTargetView(backBuffer, nullptr, &this->m_render_target_view);
	}
	SAFE_RELEASE(backBuffer);
	// this->m_device->CreateRenderTargetView(&this->m_texture, &renderTargetViewDesc, m_render_target_view);

	// this->m_render_target_texture = new RenderTargerTexture(this->m_device, 2560, 1440);
	//this->m_render_target_texture = new RenderTargerTexture(this->m_device, this->m_window_width, this->m_window_height);
	// �󶨳���Ŀ��
	this->m_context->OMSetRenderTargets(1, &this->m_render_target_view, nullptr);

	// ͸��
	ID3D11BlendState* blendState;
	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;
	blendDesc.RenderTarget->BlendEnable = TRUE; // �Ƿ������
	blendDesc.RenderTarget->SrcBlend = D3D11_BLEND_SRC_ALPHA; // ��Դͼ�� alpha ��Ϊ src rgb �Ļ������
	blendDesc.RenderTarget->DestBlend = D3D11_BLEND_INV_SRC_ALPHA; // ��Դͼ�� 1-alpha ��Ϊ dst rgb �Ļ������
	blendDesc.RenderTarget->BlendOp = D3D11_BLEND_OP_ADD; // ������Ӳ���
	blendDesc.RenderTarget->SrcBlendAlpha = D3D11_BLEND_ZERO; // 
	blendDesc.RenderTarget->DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget->BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget->RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL; // ����д���λ��
	this->m_device->CreateBlendState(&blendDesc, &blendState);
	const FLOAT BlendFactor[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
	this->m_context->OMSetBlendState(blendState, BlendFactor, 0xffffffff);
	SAFE_RELEASE(blendState);

	// ����������󶨵��ܵ��Ĺ�դ���׶�
	D3D11_VIEWPORT viewPort;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	viewPort.Width = _window_width;
	viewPort.Height = _window_height;
	viewPort.MinDepth = 0.0f;
	viewPort.MaxDepth = 1.0f;
	this->m_context->RSSetViewports(1, &viewPort);

	// ����������
	this->createTriangle2D();
	// ����Բ��
	// this->createArc2D();
}

void ZDSJ::MyDx11::render()
{
	const float bg_color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	this->m_context->ClearRenderTargetView(this->m_render_target_view, bg_color);
	this->draw();
}

void ZDSJ::MyDx11::endRender()
{
	this->m_swap_chain->Present(0, 0);
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
	for (auto item : this->m_draw_able) {
		delete item;
	}
	SAFE_RELEASE(this->m_render_target_view);
	SAFE_RELEASE(this->m_context);
	SAFE_RELEASE(this->m_swap_chain);
	SAFE_RELEASE(this->m_device);
}

void ZDSJ::MyDx11::draw()
{
	for (auto item : this->m_draw_able) {
		item->draw(this->m_context);
	}
}

void ZDSJ::MyDx11::createTriangle2D()
{
	// this->m_draw_able.push_back(new ZDSJ::Triangle2DDrawAble(this->m_device, this->m_context));
	auto triangle = (new ZDSJ::Triangle2DDrawAble(this->m_device, this->m_context))->setTranslationX(0.5f);
	this->m_draw_able.push_back(triangle->addAnimation(ZDSJ::DrawAbleAnimation::rotationZAnimation(360.f, 5000, 60, true)));
	this->m_draw_able.push_back(new ZDSJ::Triangle2DDrawAble(this->m_device, this->m_context));
	this->m_draw_able.push_back((new ZDSJ::Rectangle2DDrawAble(this->m_device, this->m_context))->setTranslationX(-0.8f));
}
