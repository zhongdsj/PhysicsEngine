# include <Application.h>
# include <Context.h>
# include <ApplicationWindowInterface.h>
# include <Dx11Interface.h>
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
	this->m_window = std::shared_ptr<ApplicationWindowInterface>(ZDSJ::createWindow(L"引擎", Context_Instance->getWindowX(), Context_Instance->getWindowY(), Context_Instance->getWindowWidth(), Context_Instance->getWindowHeight()));
	ZDSJ::createDx11(this->m_window->getHandle(), Context_Instance->getWindowWidth(), Context_Instance->getWindowHeight(), 0);
	this->m_console = std::shared_ptr<ConsoleInterface>(ZDSJ::createConsole(this->m_window->getHandle()));
	this->m_tick_component.push_back(this->m_console.get());
	this->m_tick_component.push_back(Context_Instance->getDx11());
}

void ZDSJ::Application::run() const
{
	Log_Info("start message loop");
	float accumulator = 0.0f;      // 时间累积器
	float delta_time = 0.0f;
	while(true)
	{
		delta_time = Context_Instance->getDeltaTime();
		accumulator += delta_time;
		// 3. 固定步长跑逻辑帧（核心！）
		while (accumulator >= fixed_logic_step)
		{
			Context_Instance->getKeyBoard()->frameUpdate();
			this->logicTick();
			accumulator -= fixed_logic_step;
		}
		if (this->m_window != nullptr)
		{
			MSG msg;
			if(!this->m_window->getMessage(msg))
			{
				break;
			}
		}
		// 渲染帧
		this->tick(delta_time);
	}
	Log_Info("message loop end");
}

ZDSJ::Application::~Application()
{
	Log_Info("exit Application");
}

void ZDSJ::Application::tick(float _use_time) const
{
	Context_Instance->getDx11()->beginTick();
	auto start_time = std::chrono::high_resolution_clock::now();
	for (auto item : this->m_tick_component)
	{
		item->tick(_use_time);
	}
	Context_Instance->getDx11()->endTick();
	auto end_time = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsed = end_time - start_time;
	float current_frame_dt = elapsed.count();
	Context_Instance->setDeltaTime(current_frame_dt);
	Keyboard_Instance->frameUpdate();
}

void ZDSJ::Application::logicTick() const
{
	Keyboard_Instance->logicSyncEvent();
	for (auto item : this->m_tick_component)
	{
		item->logicTick(Context_Instance->getSpeed());
	}
}
