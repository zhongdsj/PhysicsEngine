# include <Dx11Interface.h>
# include <Context.h>

void ZDSJ::Dx11Interface::registerToContext()
{
	Context_Instance->registerDx11(this);
}
