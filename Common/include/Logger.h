# pragma once
# include <memory>
# include "CommonExport.h"
# include "CommonMacro.h"
# include "spdlog/logger-inl.h"

namespace ZDSJ
{
#define Log_Trace(...)       Logger::getInstance().logger().get()->trace(__VA_ARGS__)
#define Log_Debug(...)       Logger::getInstance().logger().get()->debug(__VA_ARGS__)
#define Log_Info(...)        Logger::getInstance().logger().get()->info(__VA_ARGS__)
#define Log_Warn(...)        Logger::getInstance().logger().get()->warn(__VA_ARGS__)
#define Log_Error(...)       Logger::getInstance().logger().get()->error(__VA_ARGS__)
#define Log_Critical(...)    Logger::getInstance().logger().get()->critical(__VA_ARGS__)

	class Common_Api Logger
	{
	public:
		static Logger& getInstance();
		static std::string wchar_to_char(const std::wstring& _str);
		Delete_Copy_And_Move(Logger);
		std::shared_ptr<spdlog::logger> logger() { return m_logger; }
	private:
		Logger();
		std::shared_ptr<spdlog::logger> m_logger;
		~Logger() = default;
	};
}
