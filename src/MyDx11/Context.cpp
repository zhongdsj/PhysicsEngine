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
}

ZDSJ::Context::~Context()
{
	delete this->m_camera;
	delete this->m_keyboard;
	delete this->m_command;
	delete this->m_ring_buffer;
}

