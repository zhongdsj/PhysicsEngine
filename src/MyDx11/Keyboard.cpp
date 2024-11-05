# include <MyDx11/Keyboard.h>
# include <MyDx11/Context.h>
# include <imgui/imgui.h>

ZDSJ::Keyboard::Keyboard(ZDSJ::Context* _context)
{
	this->m_mouse_drag = new ZDSJ::Point(-1.0f, -1.0f);
	_context->command()->registerCommand("keyboard", "", "keyboard description", [&](const std::string& _data) -> bool {
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
	std::string mod;
	std::string key;
	for (const auto& mod_map : this->m_keyboard_handle) {
		for (const auto& item : mod_map.second) {
			mod = this->keyToString(static_cast<ZDSJ::Key>(mod_map.first));
			key = this->keyToString(static_cast<ZDSJ::Key>(item.first));
			oss << "\t- ";
			if (!mod.empty()) {
				oss << mod << "+";
			}
			oss << key << ": " << item.second.first << "\n";
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
	const auto mod_map = this->m_keyboard_handle.find(this->m_mod);
	if (mod_map == this->m_keyboard_handle.end()) {
		return true;
	}
	if (_key == ZDSJ::Key::mouse_wheel) {
		// 鼠标滚轮事件
	}
	const auto key = mod_map->second.find(_key);
	if (key == mod_map->second.end()) {
		return true;
	}
	if (this->m_mod == 0) {
		// 非组合键，需要判断imgui是否截取输入
		if (ImGui::GetIO().WantCaptureKeyboard) {
			return false;
		}
	}
	key->second.second(_data);
	return true;
}

std::string ZDSJ::Keyboard::keyToString(Key _key) const
{
	std::ostringstream oss;
	switch (_key)
	{
	case ZDSJ::mouse_drag:
		oss << "mouse drag";
		break;
	case ZDSJ::mouse_wheel:
		oss << "mouse wheel";
		break;
	case ZDSJ::nothing:
		// result = "nothing";
		break;
	case ZDSJ::mouse_left:
		oss << "mouse left";
		break;
	case ZDSJ::mouse_right:
		oss << "mouse right";
		break;
	case ZDSJ::mouse_middle:
		oss << "mouse middle";
		break;
	case ZDSJ::ctrl:
		oss << "ctrl";
		break;
	case ZDSJ::space:
		oss << "space";
		break;
	case ZDSJ::tilde:
		oss << "`";
		break;
	case mouse_move:
		oss << "mouse move";
		break;
	default:
		oss << static_cast<char>(_key);
		break;
	}
	return oss.str();
}

ZDSJ::Point ZDSJ::Keyboard::mouseDrag()
{
	return *this->m_mouse_drag;
}

void ZDSJ::Keyboard::mouseDrag(short _x, short _y)
{
	if (this->m_mouse_drag->x != -1.0f && this->m_mouse_drag->x != -1.0f) {
		this->execKeyboard(ZDSJ::Key::mouse_drag, this->mergeShortsToFloat(_x, _y));
	}
	this->m_mouse_drag->x = _x;
	this->m_mouse_drag->y = _y;
}

void ZDSJ::Keyboard::mouseDragReset()
{
	this->m_mouse_drag->x = -1.0f;
	this->m_mouse_drag->y = -1.0f;
}

float ZDSJ::Keyboard::mergeShortsToFloat(short first, short second)
{
	float result = (second << 16) | first;
	return result;
}

void ZDSJ::Keyboard::splitFloatToShorts(float value, short& first, short& second)
{
	unsigned __int64 temp = static_cast<unsigned __int64>(value);
	first = temp & 0xffff;
	second = (temp >> 16) & 0xffff;
}

bool ZDSJ::Keyboard::registerKeyboard(int _mod, int _key, const std::string& _description, HandlerFunc _handle_func)
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
