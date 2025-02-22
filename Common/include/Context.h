# pragma once
# include <memory>
# include "CommonExport.h"
# include "CommonMacro.h"

# define Context_Instance ZDSJ::Context::getInstance()

namespace ZDSJ
{
	class Config;

	class Common_Api Context
	{
	public:
		static Context* getInstance();
		Delete_Copy_And_Move(Context);


	protected:
		/**
		 * @brief 析构函数
		 */
		~Context();
	private:
		/**
		 * @brief 构造函数
		 */
		Context();
		ReadOnly(std::shared_ptr<Config>, config, nullptr, Config)
		ReadWrite(float, window_width, 800.0f, WindowWidth)
		ReadWrite(float, window_height, 600.0f, WindowHeight)

	};
}
