# include <Context.h>
# include <Config.h>

ZDSJ::Context* ZDSJ::Context::getInstance()
{
	static ZDSJ::Context instance;
	return &instance;
}

ZDSJ::Context::Context()
{
	Log_Info("init context");
	this->m_delta_time = 0.0f;
	this->m_speed = 1.0f;
	this->readFromConfig();
	Log_Info("context init complete");
}

void ZDSJ::Context::registerDx11(Dx11Interface* _ptr)
{
	this->m_dx11 = _ptr;
}

ZDSJ::Keyboard* ZDSJ::Context::getKeyBoard()
{
	return Keyboard::getInstance();
}

void ZDSJ::Context::readFromConfig()
{
	// 读取配置文件
	Log_Info("read configuration");
	auto root = Config_Instance->root();
	root["x"] >> this->m_window_x;
	root["y"] >> this->m_window_y;
	root["width"] >> this->m_window_width;
	root["height"] >> this->m_window_height;
}

void ZDSJ::Context::writeToConfig() const
{
	// 写入配置文件
	Log_Info("write configuration");
	auto root = Config_Instance->root();
	root["x"] << this->m_window_x;
	root["y"] << this->m_window_y;
	root["width"] << this->m_window_width;
	root["height"] << this->m_window_height;
}

void ZDSJ::Context::registerWindow(ApplicationWindowInterface* _window)
{
	this->m_window = _window;
}

ZDSJ::Context::~Context()
{
	delete this->m_dx11;
	this->writeToConfig();
}

float ZDSJ::Context::windowRate() const
{
	return static_cast<float>(this->m_window_width) / this->m_window_height;
}
