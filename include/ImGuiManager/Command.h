# pragma once
# include <string>
# include <vector>
# include <unordered_map>
# include <functional>
struct ImGuiInputTextCallbackData;

namespace ZDSJ {
	class Context;

	class Command {
		friend Context;
	public:
		using HandlerFunc = std::function<bool(std::string& _data)>;
		// -method: registeCommand
		// -brief: 注册控制台信息
		// -param: const std::string& _category 命令模块类别
		// -param: const std::string& _operation 命令名称
		// -param: const std::string& _description 命令描述
		// -param: bool(std::string& _data) _handle_func 命令触发回调， true表示已处理
		bool registeCommand(const std::string& _category, const std::string& _operation, const std::string& _description, HandlerFunc _handle_func);
		int textEditCallback(ImGuiInputTextCallbackData* _data);
		void execCommand(std::string& _command);
		void write(const std::string& _message);
	private:
		std::vector<std::string> m_history;
		std::string m_help;
		short m_max_history = 32;
		short m_history_pos = 0;
		std::unordered_map<std::string, std::unordered_map<std::string, std::pair<std::string, HandlerFunc>>> m_command_handlers;

		Command(Context* _context);
		void exec(std::string& _category, std::string& _operation, std::string& _data);
		void help();

		~Command();
	};

}