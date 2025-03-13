# pragma once
# include <CommonMacro.h>
# include <vector>
# include <ComponentInterface.h>

namespace ZDSJ
{
	class ApplicationWindowInterface;
	class ConsoleInterface;
	class Application
	{
	public:
		Application();

		Delete_Copy_And_Move(Application);

		void run() const;

		~Application();

	private:
		ApplicationWindowInterface* m_window = nullptr;
		ConsoleInterface* m_console = nullptr;
		std::vector<ComponentInterface*> m_tick_component;

		void tick(float _use_time) const;
	};
}
