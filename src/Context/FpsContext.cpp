# include <Context/FpsContext.h>

ZDSJ::FpsContext* ZDSJ::FpsContext::fps_context = nullptr;

void ZDSJ::FpsContext::initFpsContext()
{
	fps_context = new FpsContext();
}
