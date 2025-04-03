# pragma once
# include <CommonMacro.h>
# include <vector>
# include <ComponentInterface.h>
#include <memory>

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
		std::shared_ptr<ApplicationWindowInterface> m_window = nullptr;
		std::shared_ptr<ConsoleInterface> m_console = nullptr;
		std::vector<ComponentInterface*> m_tick_component;

		void tick(float _use_time) const;
	};
}
