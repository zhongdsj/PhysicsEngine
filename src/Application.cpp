# include <Application.h>
# include <Context.h>
# include <ApplicationWindowInterface.h>
# include <Dx11Interface.h>
# include <ConsoleInterface.h>
# include <Slot.h>

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
	this->m_window = std::shared_ptr<ApplicationWindowInterface>(ZDSJ::createWindow(L"引擎", Context_Instance->getWindowX(), Context_Instance->getWindowY(), Context_Instance->getWindowWidth(), Context_Instance->getWindowHeight()));
	ZDSJ::createDx11(this->m_window->getHandle(), Context_Instance->getWindowWidth(), Context_Instance->getWindowHeight(), 0);
	this->m_console = std::shared_ptr<ConsoleInterface>(ZDSJ::createConsole(this->m_window->getHandle()));
	this->m_tick_component.push_back(this->m_console.get());
}

void ZDSJ::Application::run() const
{
	Log_Info("start message loop");
	while(true)
	{
		Context_Instance->getDx11()->beginTick();
		if (this->m_window != nullptr)
		{
			MSG msg;
			if(!this->m_window->getMessage(msg))
			{
				break;
			}
		}
		this->tick(1.0);
		Context_Instance->getDx11()->endTick();
	}
	Log_Info("message loop end");
}

ZDSJ::Application::~Application()
{
	Log_Info("exit Application");
}

void ZDSJ::Application::tick(float _use_time) const
{
	Context_Instance->getDx11()->tick(_use_time);
	for (auto item : this->m_tick_component)
	{
		item->tick(_use_time);
	}
}
