# include <ConsoleByImgui.h>
# include <d3d11.h>
# include <imgui/imgui.h>
# include <imgui/imgui_impl_win32.h>
# include <imgui/imgui_impl_dx11.h>
# include <imgui/imgui.h>
# include <Context.h>
# include <ApplicationWindowInterface.h>
# include <KeyEnum.h>
# include <Slot.h>
# include <Command.h>

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

ZDSJ::ConsoleByImgui::ConsoleByImgui(HWND _handle)
{
	// TODO 改为创建子窗口, 在子窗口中创建dx11与ImGui, 将控制台使用的dx11与渲染端使用的dx11分开
	Log_Info("[console]: create console");
	// this->createDeviceD3D(_handle);
	this->m_device = Context_Instance->getDx11()->device();
	this->m_context = Context_Instance->getDx11()->context();
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	this->registerFonts();
	ImGui_ImplWin32_Init(_handle);
	ImGui_ImplDX11_Init(this->m_device, this->m_context);
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	auto window = Context_Instance->getWindow();
	window->addHandleMessage("console", std::bind(&ConsoleInterface::messageHandle, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	if(window != nullptr)
	{
		this->m_slots.push_back(window->connect(ZDSJ::KEY::wavy, new Slot([this](KeyOperation _operation)
		{
			switch (_operation)
			{
			case KeyOperation::up:
				this->m_show.store(!this->m_show.load());
				break;
			default: 
				break;
			}
		}, "波浪键切换控制台显隐")));
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
	ImGui::SameLine();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	for (auto log : Log_Buffer)
	{
		ImGui::TextUnformatted(log.data(), log.data() + log.size());
	}
	ImGui::EndChild();
	ImGui::Separator();
	char command[128] = "";
	if (ImGui::InputText("command", command, 128, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CallbackHistory, &textEditCallbackStub, reinterpret_cast<void*>(this))) {
		std::string temp(command);
		Command::getInstance().execCommand(temp);
		ImGui::SetKeyboardFocusHere(-1);
	}
	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

int ZDSJ::ConsoleByImgui::textEditCallbackStub(ImGuiInputTextCallbackData* _data)
{
	return 0;
}

void ZDSJ::ConsoleByImgui::registerFonts()
{
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	// 启用UTF-8支持（关键）
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.Fonts->AddFontDefault(); // 保留默认字体（用于英文）
	std::string font_path = "../../fonts/MSYH.TTC";
	// 字体配置：指定中文字符范围+字体大小
	ImFontConfig font_cfg;
	font_cfg.MergeMode = true; // 合并到默认字体（同时显示中英文）
	font_cfg.PixelSnapH = true;
	// 中文简体字符范围（0x4E00~0x9FFF）+ 常用符号
	static const ImWchar chinese_ranges[] = {
		0x0020, 0x00FF, // 基本ASCII
		0x4E00, 0x9FFF, // 中文简体
		0xFF00, 0xFFEF, // 全角符号
		0x0000, 0x0000  // 结束标记
	};

	// 加载字体（字体大小设为16，可根据需求调整）
	ImFont* chinese_font = io.Fonts->AddFontFromFileTTF(
		font_path.c_str(),
		16.0f,
		&font_cfg,
		chinese_ranges
	);
}

ZDSJ::ConsoleInterface* ZDSJ::createConsole(HWND _handle)
{
	return new ZDSJ::ConsoleByImgui(_handle);
}
