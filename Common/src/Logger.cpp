# include <codecvt>
# include <Logger.h>
# include <spdlog/spdlog.h>
# include <spdlog/sinks/rotating_file_sink.h>

ZDSJ::Logger& ZDSJ::Logger::getInstance()
{
	static Logger instance;
	return instance;
}

std::string ZDSJ::Logger::wchar_to_char(const std::wstring& _str)
{
	try
	{
		std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
		return converter.to_bytes(_str);
	}
	catch (const std::range_error& e) {
		Log_Error("宽字节转换失败");
		return "";
	}
}

ZDSJ::Logger::Logger()
{
	try {
		/* file sink */
		auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/log.log", 1024*1024, 1);
		file_sink->set_level(spdlog::level::debug);
		file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] %v");

		/* Sink组合 */
		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(file_sink);
		this->m_logger = std::make_shared<spdlog::logger>("multi-sink", begin(sinks), end(sinks));
		this->m_logger->set_level(spdlog::level::trace);
	}
	catch (const spdlog::spdlog_ex& ex) {
		perror("spdlog init error.");
	}
}
