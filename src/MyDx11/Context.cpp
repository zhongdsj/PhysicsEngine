# include <MyDx11/Context.h>
# include <MyDx11/MyDx11.h>

ZDSJ::Context* ZDSJ::Context::getInstance()
{
	static ZDSJ::Context _instance;
	return &_instance;
}

ZDSJ::MyDx11* ZDSJ::Context::dx11() const
{
	return this->m_dx11;
}

void ZDSJ::Context::dx11(MyDx11* _dx11)
{
	this->m_dx11 = _dx11;
}

ZDSJ::Context::Context() {
	this->m_ring_buffer = new ZDSJ::RingBuffer<std::string>(32, [](std::string& _value) {});
	this->m_command = new ZDSJ::Command(this);
	this->m_keyboard = new ZDSJ::Keyboard(this);
	this->m_camera = new ZDSJ::Camera(this, this->m_window_rate);
	this->m_keyboard->registeKeyboard(ZDSJ::Key::nothing, ZDSJ::Key::mouse_move, "change mouse to word", [&](float _data) {
		short x;
		short y;
		this->m_keyboard->splitFloatToShorts(_data, x, y);
		ZDSJ::Point word_pos = this->m_camera->viewPosToWordPos(ZDSJ::Point(x, y));
		this->m_mouse_word = word_pos;
	});
	this->m_keyboard->registeKeyboard(ZDSJ::Key::nothing, ZDSJ::Key::mouse_left, "mouse click in frame", [&](float _data) {
		this->mouseClick(true);
	});
	this->m_keyboard->registeKeyboard(ZDSJ::Key::nothing, ZDSJ::Key::mouse_right, "mouse right click in frame", [&](float _data) {
		this->mouseRight(true);
	});
	// 注册command
	this->m_command->registeCommand("word", "run", "stop word if data is false, continue with true", [&](std::string& _data) {
		if (_data == "true") {
			this->wordActive(true);
		}
		else if (_data == "false") {
			this->wordActive(false);
		}
		else {
			return false;
		}
		return true;
	});
	// 注册keyboard
	this->m_keyboard->registeKeyboard(ZDSJ::Key::nothing, ZDSJ::Key::space, "pause word", [&](float _data) {
		this->wordActive(!this->wordActive());
	});
}

ZDSJ::Context::~Context()
{
	delete this->m_camera;
	delete this->m_keyboard;
	delete this->m_command;
	delete this->m_ring_buffer;
}

