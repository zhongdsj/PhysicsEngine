# include <codecvt>
# include <spdlog/spdlog.h>
# include <spdlog/sinks/rotating_file_sink.h>
# include <Logger.h>
# include <Config.h>

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

std::vector<std::string> ZDSJ::Logger::getLog() const
{
	std::vector<std::string> result = {};
	if (this->m_ringBufferSink.get() != nullptr)
	{
		result = this->m_ringBufferSink->last_formatted();
	}
	return result;
}

ZDSJ::Logger::Logger()
{
	try {
		auto root = Config_Instance->root();
		// 读取日志配置
		root["logger"]["buffer_item"] >> this->m_log_config.buffer_item;
		root["logger"]["log_name"] >> this->m_log_config.log_name;
		root["logger"]["max_size"] >> this->m_log_config.max_size;
		root["logger"]["max_files"] >> this->m_log_config.max_files;
		/* file sink */
		auto file_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/log.log", m_log_config.max_size, m_log_config.max_files);
		file_sink->set_level(spdlog::level::debug);
		file_sink->set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%t] %v");
		this->m_ringBufferSink = std::make_shared<spdlog::sinks::ringbuffer_sink_mt>(m_log_config.buffer_item);
		/* Sink组合 */
		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(file_sink);
		sinks.push_back(this->m_ringBufferSink);
		this->m_logger = std::make_shared<spdlog::logger>(m_log_config.log_name, begin(sinks), end(sinks));
		this->m_logger->set_level(spdlog::level::trace);
	}
	catch (const spdlog::spdlog_ex& ex) {
		perror("spdlog init error.");
	}
}
