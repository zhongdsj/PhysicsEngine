# pragma once
# include <ryml_all.hpp>
# include "CommonMacro.h"

# define Config_Instance ZDSJ::Config::getInstance()

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
		static Config* getInstance();
		Delete_Copy_And_Move(Config);
		c4::yml::NodeRef root();
	private:
		/**
		 * @brief 构造函数
		 */
		Config(const char* filename);
		c4::yml::Tree m_tree;
		std::string m_buffer;
		std::string m_file_name;
	protected:
		/**
		 * @brief 析构函数
		 */
		~Config();
	};
}
