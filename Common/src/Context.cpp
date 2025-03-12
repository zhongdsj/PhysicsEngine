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
	this->m_config = std::make_shared<ZDSJ::Config>("./config.json");
	this->readFromConfig();
	Log_Info("context init complete");
}

void ZDSJ::Context::readFromConfig()
{
	// 读取配置文件
	Log_Info("read configuration");
	auto root = this->m_config->root();
	root["x"] >> this->m_window_x;
	root["y"] >> this->m_window_y;
	root["width"] >> this->m_window_width;
	root["height"] >> this->m_window_height;
}

void ZDSJ::Context::writeToConfig() const
{
	// 写入配置文件
	Log_Info("write configuration");
	auto root = this->m_config->root();
	root["x"] << this->m_window_x;
	root["y"] << this->m_window_y;
	root["width"] << this->m_window_width;
	root["height"] << this->m_window_height;
}

ZDSJ::Context::~Context()
{
	this->writeToConfig();
}
