# pragma once
# include <memory>
# include "CommonExport.h"
# include "CommonMacro.h"
# include "Logger.h"

# define Context_Instance ZDSJ::Context::getInstance()

namespace ZDSJ
{
	class ApplicationWindowInterface;
	class Common_Api Context
	{
		friend ApplicationWindowInterface;
	public:
		static Context* getInstance();
		Delete_Copy_And_Move(Context);

	protected:
		/**
		 * @brief 析构函数
		 */
		~Context();

		void registerWindow(ApplicationWindowInterface* _window);
	private:
		/**
		 * @brief 构造函数
		 */
		Context();
		ReadOnly(ApplicationWindowInterface*, window, nullptr, Window)
		ReadWrite(int, window_x, 200, WindowX)
		ReadWrite(int, window_y, 100, WindowY)
		ReadWrite(int, window_width, 800, WindowWidth)
		ReadWrite(int, window_height, 600, WindowHeight)
		ReadOnly(LoggerConfig, log_config, {}, LoggerConfig)

		void readFromConfig();

		void writeToConfig() const;
	};
}
