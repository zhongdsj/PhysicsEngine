# include <ImGuiManager/ImGuiManager.h>
# include <imgui/imgui.h>
# include <imgui/imgui_impl_win32.h>
# include <imgui/imgui_impl_dx11.h>
# include <MyDx11/Context.h>
# include <ImGuiManager/Command.h>


ZDSJ::ImGuiManager::ImGuiManager(HWND _hwnd, ID3D11Device* _device, ID3D11DeviceContext* _context)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(_hwnd);
	ImGui_ImplDX11_Init(_device, _context);
	
	// ctrl+F 显示fps
	ZDSJ::Context::getInstance()->Keyboard()->registeKeyboard(ZDSJ::Key::ctrl, 'F', "show fps", [&](float _ignore) {
		this->m_show_fps = !this->m_show_fps;
	});

	// `打开控制台
	ZDSJ::Context::getInstance()->Keyboard()->registeKeyboard(ZDSJ::Key::nothing, ZDSJ::Key::tilde, "open console", [&](float _ignore) {
		this->m_show_console = !this->m_show_console;
		// ImGui::getwindow
	});

	this->m_window_flag |= ImGuiWindowFlags_NoTitleBar;
	this->m_window_flag |= ImGuiWindowFlags_NoMove;
	this->m_window_flag |= ImGuiWindowFlags_NoResize;
	this->m_window_flag |= ImGuiWindowFlags_NoSavedSettings;
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
	// 显示中文, 输入中文时卡顿严重
	// auto f = ImGui::GetIO().Fonts->AddFontFromFileTTF("source/fonts/simkai.ttf", 13.0f, nullptr, ImGui::GetIO().Fonts->GetGlyphRangesChineseSimplifiedCommon());
	ImGui::GetIO().Fonts->AddFontFromFileTTF("source/fonts/FiraCode-Medium.ttf", 16.0f);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT ZDSJ::ImGuiManager::handelMessage(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(handle, msg, wParam, lParam))
		return true;
	return false;
}

void ZDSJ::ImGuiManager::render()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (this->m_show_fps || this->m_show_console) {
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Appearing);
		ImGui::SetNextWindowSize(ImVec2(0, 0));
		ImGui::Begin("console", NULL, this->m_window_flag);
		if (this->m_show_fps) {
			ImGui::Text("fps: %.2f, frame_time: %.5fms", ZDSJ::Context::getInstance()->fps(), ZDSJ::Context::getInstance()->useTime());
			ZDSJ::float4 camera_pos = ZDSJ::Context::getInstance()->camera()->cameraPos();
			ImGui::Text("fov: %.1f, camera_pos: { %.1f, %.1f, %.1f }", ZDSJ::Context::getInstance()->camera()->fov(), camera_pos.x, camera_pos.y, camera_pos.z);
		}
		
		// 控制台输入/输出
		if (this->m_show_console) {
			ZDSJ::Context::getInstance()->ringBuffer()->enablePush(true);
			ImGui::Separator();
			ImGui::BeginChild("ScrollingRegion", ImVec2(ZDSJ::Context::getInstance()->windowWidth() * 0.6, ZDSJ::Context::getInstance()->windowHeight() * 0.6), false, ImGuiWindowFlags_HorizontalScrollbar);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
			ImGui::PushTextWrapPos(ImGui::GetCursorPos().x + ImGui::GetWindowWidth() - ImGui::GetStyle().ScrollbarSize);
			ZDSJ::Context::getInstance()->ringBuffer()->forEeach([](const std::string& _message) {
				ImGui::TextUnformatted(_message.data(), _message.data() + _message.size());
			});
			ImGui::PopTextWrapPos();
			if (ImGui::GetScrollY() == ImGui::GetScrollMaxY()) {
				this->m_auto_console_bottom = true;
			}
			else {
				this->m_auto_console_bottom = false;
			}
			if (m_auto_console_bottom) {
				ImGui::SetScrollHereY(1.0f);
			}
			ImGui::PopStyleVar();
			ImGui::EndChild();
			ImGui::Separator();
			char command[128] = "";
			if (ImGui::InputText("command", command, 128, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_CallbackHistory, &textEditCallbackStub, reinterpret_cast<void*>(this))) {
				std::string temp(command);
				ZDSJ::Context::getInstance()->command()->execCommand(temp);
				ImGui::SetKeyboardFocusHere(-1);
			}
		}
		else {
			ZDSJ::Context::getInstance()->ringBuffer()->enablePush(false);
		}

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void ZDSJ::ImGuiManager::showFps(bool _show)
{
	this->m_show_fps = _show;
}

int ZDSJ::ImGuiManager::textEditCallbackStub(ImGuiInputTextCallbackData* _data)
{
	return ZDSJ::Context::getInstance()->command()->textEditCallback(_data);
}

ZDSJ::ImGuiManager::~ImGuiManager()
{
	ImGui::PopStyleColor(2);
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
