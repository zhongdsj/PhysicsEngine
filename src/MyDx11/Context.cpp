#include <MyDx11/Context.h>

ZDSJ::Context* ZDSJ::Context::getInstance()
{
	static ZDSJ::Context _instance;
	return &_instance;
}

ZDSJ::Context::Context() {
	this->m_ring_buffer = new ZDSJ::RingBuffer<std::string>(32, [](std::string& _value) {});
	this->m_command = new ZDSJ::Command(this);
	this->m_keyboard = new ZDSJ::Keyboard(this);
	this->m_camera = new ZDSJ::Camera(this, this->m_window_rate);
}

ZDSJ::Context::~Context()
{
	delete this->m_camera;
	delete this->m_command;
	delete this->m_ring_buffer;
}

