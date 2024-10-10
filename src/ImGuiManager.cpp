# include <ImGuiManager.h>
# include <imgui/imgui.h>
# include <imgui/imgui_impl_win32.h>
# include <imgui/imgui_impl_dx11.h>


ZDSJ::ImGuiManager::ImGuiManager(HWND _hwnd, ID3D11Device* _device, ID3D11DeviceContext* _context)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplWin32_Init(_hwnd);
	ImGui_ImplDX11_Init(_device, _context);
	this->m_window_flag |= ImGuiWindowFlags_NoTitleBar;
	this->m_window_flag |= ImGuiWindowFlags_NoMove;
	this->m_window_flag |= ImGuiWindowFlags_NoResize;
	this->m_window_flag |= ImGuiWindowFlags_NoSavedSettings;
	ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.2f, 0.2f, 0.5f));
	ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT ZDSJ::ImGuiManager::handelMessage(HWND handle, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(handle, msg, wParam, lParam))
		return true;
}

void ZDSJ::ImGuiManager::render(float _fps)
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	// 处理ctrl
	this->captureModCtrl();
	// 处理控制台开启
	this->captureGraveAccent();
	if (this->m_show_fps || this->m_console) {
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Appearing);
		ImGui::SetNextWindowSize(ImVec2(0, 0));
		ImGui::Begin("console", NULL, this->m_window_flag);
		
		if (this->m_show_fps) {
			ImGui::Text("fps: %.2f, frame_time: %f", _fps, 0.0f);
		}

		// TODO 控制台输入/输出
		if (this->m_console) {
			ImGui::Separator();
			auto a = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
			ImGui::BeginChild("ScrollingRegion", ImVec2(0, ImGui::GetFontSize() * 20), false, ImGuiWindowFlags_HorizontalScrollbar);
			ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1));
			
			ImGuiListClipper clipper;
			char command[128] = "";
			auto t = "Console features are being developed, you can press ctrl + mouseleft output this text";
			for (int i = 0; i < this->m_num; ++i) {
				ImGui::TextUnformatted(t, t + strlen(t));
			}
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
			// ImGui::InputText("input text", command, 128, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory);
			if (ImGui::InputText("command", command, 128, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_AutoSelectAll)) {

			}
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

void ZDSJ::ImGuiManager::captureModCtrl()
{
	if (ImGui::GetIO().KeyCtrl) {
		// ctrl按下
		this->captureKeyFPress();
		if (ImGui::IsKeyPressed(ImGuiKey_MouseLeft)) {
			this->m_num++;
		}
	}
}

void ZDSJ::ImGuiManager::captureKeyFPress()
{
	if (ImGui::IsKeyPressed(ImGuiKey_F)) {
		// F按下
		this->m_show_fps = !this->m_show_fps;
	}
}

void ZDSJ::ImGuiManager::captureGraveAccent()
{
	if (ImGui::IsKeyPressed(ImGuiKey_GraveAccent)) {
		// `按下
		this->m_console = !this->m_console;
	}
}

ZDSJ::ImGuiManager::~ImGuiManager()
{
	// ImGui::PopStyleVar();
	ImGui::PopStyleColor(2);
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
