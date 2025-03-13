# include "Application.h"
# include "Context.h"
# include "ApplicationWindowInterface.h"
# include <ConsoleInterface.h>

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
	this->m_console = ZDSJ::createConsole(this->m_window->getHandle());
	this->m_window->addHandelMessage("console", std::bind(&ConsoleInterface::messageHandle, this->m_console, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4));
	this->m_tick_component.push_back(this->m_console);
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
		this->tick(1.0);
	}
	Log_Info("message loop end");
}

ZDSJ::Application::~Application()
{
	delete this->m_console;
	delete this->m_window;
	Log_Info("exit engine");
}

void ZDSJ::Application::tick(float _use_time) const
{
	for (auto item : this->m_tick_component)
	{
		item->tick(_use_time);
	}
}
