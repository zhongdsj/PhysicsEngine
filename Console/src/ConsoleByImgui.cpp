# include <ConsoleByImgui.h>
# include <d3d11.h>
# include <imgui/imgui.h>
# include <imgui/imgui_impl_win32.h>
# include <imgui/imgui_impl_dx11.h>
# include <Context.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ZDSJ::ConsoleByImgui::ConsoleByImgui(HWND _handle)
{
	Log_Info("[console]: create console");
	this->createDeviceD3D(_handle);
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(_handle);
	ImGui_ImplDX11_Init(this->m_device.get(), this->m_context.get());

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
}

void ZDSJ::ConsoleByImgui::tick(float _use_time)
{
	this->rend();
}

LRESULT ZDSJ::ConsoleByImgui::messageHandle(HWND _handle, UINT _msg, WPARAM _w_param, LPARAM _l_param)
{
	return ImGui_ImplWin32_WndProcHandler(_handle,_msg, _w_param, _l_param);
}

bool ZDSJ::ConsoleByImgui::show()
{
	return false;
}

ZDSJ::ConsoleByImgui::~ConsoleByImgui()
{
	ImGui::PopStyleColor(2);
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ZDSJ::ConsoleByImgui::rend()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
	ImGui::SameLine();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	for (auto log : Log_Buffer)
	{
		ImGui::TextUnformatted(log.data(), log.data() + log.size());
	}
	ImGui::EndChild();
	ImGui::End();

	ImGui::Render();
	ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
	const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
	this->m_context->ClearRenderTargetView(this->m_render_target_view.get(), clear_color_with_alpha);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	this->m_swap_chain->Present(1, 0);
}

void ZDSJ::ConsoleByImgui::createDeviceD3D(HWND _handle)
{
	// 创建设备及交换链
	Log_Info("create d3d11 device and context");
	DXGI_SWAP_CHAIN_DESC swap_chain_desc;
	ZeroMemory(&swap_chain_desc, sizeof(swap_chain_desc));
	swap_chain_desc.BufferCount = 2;
	swap_chain_desc.BufferDesc.Width = 0;
	swap_chain_desc.BufferDesc.Height = 0;
	swap_chain_desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swap_chain_desc.BufferDesc.RefreshRate.Numerator = 60;
	swap_chain_desc.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swap_chain_desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_desc.OutputWindow = _handle;
	swap_chain_desc.SampleDesc.Count = 1;
	swap_chain_desc.SampleDesc.Quality = 0;
	swap_chain_desc.Windowed = TRUE;
	swap_chain_desc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	UINT create_device_flags = 0;
	IDXGISwapChain* chain;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	HRESULT result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, create_device_flags, nullptr, NULL, D3D11_SDK_VERSION, &swap_chain_desc,
	                                               &chain, &device, nullptr, &context);
	this->m_swap_chain = std::shared_ptr<IDXGISwapChain>(chain, [](IDXGISwapChain* _ptr) { if(_ptr != nullptr) _ptr->Release(); _ptr = nullptr; });
	this->m_device = std::shared_ptr<ID3D11Device>(device, [](ID3D11Device* _ptr) { if (_ptr != nullptr) _ptr->Release(); _ptr = nullptr; });
	this->m_context = std::shared_ptr<ID3D11DeviceContext>(context, [](ID3D11DeviceContext* _ptr) { if (_ptr != nullptr) _ptr->Release(); _ptr = nullptr; });

	if (result != S_OK)
	{
		Log_Error("[console]: can not create d3d11 device and context");
		return;
	}

	this->createRenderTarget();
}

void ZDSJ::ConsoleByImgui::createRenderTarget()
{
	// 创建呈现目标
	ID3D11Texture2D* back_buffer;
	ID3D11RenderTargetView* render_target;
	this->m_swap_chain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));
	if (back_buffer != nullptr) {
		this->m_device->CreateRenderTargetView(back_buffer, nullptr, &render_target);
		this->m_render_target_view = std::shared_ptr<ID3D11RenderTargetView>(render_target, [](ID3D11RenderTargetView* _ptr) { if (_ptr != nullptr) _ptr->Release(); _ptr = nullptr; });
		this->m_context->OMSetRenderTargets(1, &render_target, nullptr);
	}
	if(back_buffer != nullptr)
	{
		back_buffer->Release();
		back_buffer = nullptr;
	}
}
