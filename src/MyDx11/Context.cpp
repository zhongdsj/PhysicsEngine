#include <MyDx11/Context.h>

ZDSJ::Context* ZDSJ::Context::getInstance()
{
	static ZDSJ::Context _instance;
	return &_instance;
}
