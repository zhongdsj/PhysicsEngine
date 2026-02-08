# pragma once
# include <CommonExport.h>
# include <CommonMacro.h>
# include <string>
# include <unordered_map>
# include <SignalSender.h>
# include <sstream>


namespace ZDSJ
{
	class CommandNode;
	class Common_Api Command : public ZDSJ::SignalSender {
	public:
		static Command& getInstance();
		Slot* connect(const std::string& signal, Slot* slot) override;
		void execCommand(const std::string& _command) const;
		Delete_Copy_And_Move(Command);
	private:
		Command();
		std::unordered_map<std::string, CommandNode*> m_slots;
		static std::vector<std::string> splitPath(const std::string& path, const char delim) {
			std::vector<std::string> parts;
			std::stringstream ss(path);
			std::string part;
			while (std::getline(ss, part, delim)) {
				if (!part.empty()) {  // 跳过空串（如路径开头/结尾的/）
					parts.push_back(part);
				}
			}
			return parts;
		}
	protected:
		~Command() override = default;
		void registerSignal() override;
	};
}
