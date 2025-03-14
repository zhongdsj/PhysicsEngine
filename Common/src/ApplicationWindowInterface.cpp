# include <ApplicationWindowInterface.h>

#include <Context.h>

void ZDSJ::ApplicationWindowInterface::registerToContext()
{
	Context_Instance->registerWindow(this);
}

