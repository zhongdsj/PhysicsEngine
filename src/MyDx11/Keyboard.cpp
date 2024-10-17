# include <MyDx11/Keyboard.h>
# include <MyDx11/Context.h>
# include <imgui/imgui.h>

ZDSJ::Keyboard::Keyboard(ZDSJ::Context* _context)
{
	_context->command()->registeCommand("keyboard", "", "keyboard description", [&](std::string& _data) -> bool {
		if (_data.empty()) {
			ZDSJ::Context::getInstance()->command()->write(this->outputToCommand());
			return true;
		}
		return false;
	});
}

std::string ZDSJ::Keyboard::outputToCommand() const
{
	std::ostringstream oss;
	for (auto mod_map : this->m_keyboard_handle) {
		for (auto item : mod_map.second) {
			oss << "\t- " << mod_map.first << "+" << item.first << ": " << item.second.first << "\n";
		}
	}
	return oss.str();
}

void ZDSJ::Keyboard::mod(int _mod)
{
	this->m_mod = _mod;
}

bool ZDSJ::Keyboard::execKeyboard(int _key, float _data)
{
	auto mod_map = this->m_keyboard_handle.find(this->m_mod);
	if (mod_map == this->m_keyboard_handle.end()) {
		return true;
	}
	if (_key == ZDSJ::Key::mouse_wheel) {
		// 鼠标滚轮事件
	}
	auto key = mod_map->second.find(_key);
	if (key == mod_map->second.end()) {
		return true;
	}
	if (this->m_mod == 0) {
		// 非组合键，需要判断imgui是否截取输入
		if (ImGui::GetIO().WantCaptureKeyboard || ImGui::GetIO().WantCaptureMouse) {
			return false;
		}
	}
	key->second.second(_data);
	return true;
}

bool ZDSJ::Keyboard::registeKeyboard(int _mod, int _key, const std::string& _description, HandlerFunc _handle_func)
{
	auto mod_map = this->m_keyboard_handle.find(_mod);
	if (mod_map == this->m_keyboard_handle.end()) {
		this->m_keyboard_handle.insert(std::make_pair(_mod, std::unordered_map<int, std::pair<std::string, HandlerFunc>>()));
	}
	mod_map = this->m_keyboard_handle.find(_mod);
	auto key = mod_map->second.find(_key);
	if (key != mod_map->second.end()) {
		return false;
	}
	mod_map->second.insert(std::make_pair(_key, std::make_pair(_description, _handle_func)));
	return true;
}



ZDSJ::Keyboard::~Keyboard()
{
}
