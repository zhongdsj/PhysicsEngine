# pragma once
# include "CommonMacro.h"

namespace ZDSJ
{
	class ApplicationWindowInterface;
	class Application
	{
	public:
		Application();

		Delete_Copy_And_Move(Application);

		void run() const;

		~Application();

	private:
		ApplicationWindowInterface* m_window = nullptr;
	};
}
