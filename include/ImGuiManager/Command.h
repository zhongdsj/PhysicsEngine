# pragma once
# include <string>
# include <vector>
# include <unordered_map>
# include <functional>
struct ImGuiInputTextCallbackData;

namespace ZDSJ {
	class Context;

	class Command {
	public:
		using HandlerFunc = std::function<bool(std::string& _data)>;
		static Command* getInstance();
		bool registeCommand(const std::string& _operation, const std::string& _description, HandlerFunc _handle_func);
		int textEditCallback(ImGuiInputTextCallbackData* _data);
		void execCommand(std::string& _command);
	private:
		Context* _context = nullptr;
		std::vector<std::string> m_history;
		std::string m_help;
		short m_max_history = 32;
		short m_history_pos = 0;
		std::unordered_map<std::string, std::pair<std::string, HandlerFunc>> m_command_handlers;

		Command();
		void exec(std::string& _operation, std::string& _data);
		void help();
	protected:
		~Command();
	};

}