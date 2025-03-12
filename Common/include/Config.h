# pragma once
# include <ryml_all.hpp>
# include "CommonMacro.h"

namespace c4
{
	namespace yml
	{
		class Tree;
		class NodeRef;
	}
	
}

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

		c4::yml::NodeRef root();

		/**
		 * @brief 析构函数
		 */
		~Config();
	private:
		c4::yml::Tree m_tree;
		std::string m_buffer;
		std::string m_file_name;
	};
}
