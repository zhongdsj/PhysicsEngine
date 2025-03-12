# include "Application.h"
# include "Context.h"
# include "ApplicationWindowInterface.h"

ZDSJ::Application::Application()
{
	Log_Info(R"(
	   _______  ______   _______ _________
	/ ___   )(  __  \ (  ____ \\__    _/
	\/   )  || (  \  )| (    \/   )  (  
	    /   )| |   ) || (_____    |  |  
	   /   / | |   | |(_____  )   |  |  
	  /   /  | |   ) |      ) |   |  |  
	 /   (_/\| (__/  )/\____) ||\_)  )  
	(_______/(______/ \_______)(____/   
	-------------------------------------
			welcome to my engine
)");
	this->m_window = ZDSJ::createWindow(L"引擎", Context_Instance->getWindowX(), Context_Instance->getWindowY(), Context_Instance->getWindowWidth(), Context_Instance->getWindowHeight());
}

void ZDSJ::Application::run() const
{
	Log_Info("start message loop");
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
	Log_Info("message loop end");
}

ZDSJ::Application::~Application()
{
	delete this->m_window;
	Log_Info("exit engine");
}
