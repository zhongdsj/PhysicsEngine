# include "Application.h"

#include "Context.h"
#include "ApplicationWindowInterface.h"

ZDSJ::Application::Application()
{
	// todo 读取配置文件
	this->m_window = ZDSJ::createWindow(L"引擎", 200, 200, Context_Instance->getWindowWidth(), Context_Instance->getWindowHeight());
}

void ZDSJ::Application::run() const
{
	while(true)
	{
		if (this->m_window != nullptr)
		{
			MSG msg;
			if(!this->m_window->getMessage(msg))
			{
				break;
			}
		}
	}
}

ZDSJ::Application::~Application()
{
	delete this->m_window;
}
