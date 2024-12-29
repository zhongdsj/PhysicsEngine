# include "Context.h"

ZDSJ::Context* ZDSJ::Context::getInstance()
{
	static ZDSJ::Context instance;
	return &instance;
}

ZDSJ::Context::Context()
{
	
}

ZDSJ::Context::~Context()
{

	
}
