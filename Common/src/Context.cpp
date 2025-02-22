# include <Context.h>
# include <Config.h>
# include "spdlog/spdlog.h"

ZDSJ::Context* ZDSJ::Context::getInstance()
{
	static ZDSJ::Context instance;
	return &instance;
}

ZDSJ::Context::Context()
{
	spdlog::info("Welcome to spdlog!");
	this->m_config = std::make_shared<ZDSJ::Config>("./config.json");
}

ZDSJ::Context::~Context()
{

	
}
