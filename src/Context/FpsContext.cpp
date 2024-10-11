# include <Context/FpsContext.h>

ZDSJ::FpsContext* ZDSJ::FpsContext::getInstance()
{
	static ZDSJ::FpsContext instance;
	return &instance;
}
