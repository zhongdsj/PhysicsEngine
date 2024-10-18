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
	std::string mod;
	std::string key;
	for (auto mod_map : this->m_keyboard_handle) {
		for (auto item : mod_map.second) {
			mod = this->keyToString(ZDSJ::Key(mod_map.first));
			key = this->keyToString(ZDSJ::Key(item.first));
			oss << "\t- ";
			if (!mod.empty()) {
				oss << mod << "+";
			}
			oss << key << ": " << item.second.first << "\n";
			// << mod_map.first << "+" << item.first << ": " << item.second.first << "\n";
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
	default:
		oss << static_cast<char>(_key);
		break;
	}
	return oss.str();
}

void ZDSJ::Keyboard::mouseDrag(short _x, short _y)
{
	if (this->m_mouse_drag_x != -1.0f && this->m_mouse_drag_y != -1.0f) {
		this->execKeyboard(ZDSJ::Key::mouse_drag, this->mergeShortsToFloat(_x - this->m_mouse_drag_x, _y - this->m_mouse_drag_y));
	}
	this->m_mouse_drag_x = _x;
	this->m_mouse_drag_y = _y;
}

void ZDSJ::Keyboard::mouseDragReset()
{
	this->m_mouse_drag_x = -1.0f;
	this->m_mouse_drag_y = -1.0f;
}

float ZDSJ::Keyboard::mergeShortsToFloat(short first, short second)
{
	uint8_t bytes[4];
	bytes[0] = static_cast<uint8_t>(first & 0xFF);
	bytes[1] = static_cast<uint8_t>((first >> 8) & 0xFF);
	bytes[2] = static_cast<uint8_t>(second & 0xFF);
	bytes[3] = static_cast<uint8_t>((second >> 8) & 0xFF);

	float result;
	std::memcpy(&result, bytes, sizeof(result));
	return result;
}

void ZDSJ::Keyboard::splitFloatToShorts(float value, short& first, short& second)
{
	uint8_t bytes[4];
	std::memcpy(bytes, &value, sizeof(value));

	first = (bytes[1] << 8) | bytes[0];
	second = (bytes[3] << 8) | bytes[2];
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
