# include <ConsoleByImgui.h>
# include <d3d11.h>
# include <imgui/imgui.h>
# include <imgui/imgui_impl_win32.h>
# include <imgui/imgui_impl_dx11.h>
# include <Context.h>
# include <ApplicationWindowInterface.h>
# include <KeyEnum.h>
# include <Slot.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ZDSJ::ConsoleByImgui::ConsoleByImgui(HWND _handle)
{
	Log_Info("[console]: create console");
	// this->createDeviceD3D(_handle);
	this->m_device = Context_Instance->getDx11()->device();
	this->m_context = Context_Instance->getDx11()->context();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(_handle);
	ImGui_ImplDX11_Init(this->m_device, this->m_context);

	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	auto window = Context_Instance->getWindow();
	window->addHandleMessage("console", std::bind(&ConsoleInterface::messageHandle, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	if(window != nullptr)
	{
		this->m_slots.push_back(window->connect(ZDSJ::KEY::wavy, new Slot([this]()
		{
			this->m_show.store(!this->m_show.load());
		})));
	}
}

void ZDSJ::ConsoleByImgui::tick(float _use_time)
{
	if (this->m_show.load())
	{
		this->rend();
	}
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
	for (auto value : this->m_slots)
	{
		value->useful(false);
	}
	auto window = Context_Instance->getWindow();
	if (window != nullptr)
	{
		window->removeHandleMessage("console");
	}
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

	ImGui::Begin("Console");                          // Create a window called "Hello, world!" and append into it.
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
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

ZDSJ::ConsoleInterface* ZDSJ::createConsole(HWND _handle)
{
	return new ZDSJ::ConsoleByImgui(_handle);
}
