# pragma once
# include "CommonMacro.h"

namespace ZDSJ
{
	class Config
	{
	public:
		/**
		 * @brief 构造函数
		 */
		Config(const char* filename);
		Delete_Copy_And_Move(Config);
		/**
		 * @brief 析构函数
		 */
		~Config();
	};
}
