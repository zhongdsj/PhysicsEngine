# include <ImGuiManager/Command.h>
# include <RingBuffer.h>
# include <MyDx11/Context.h>
# include <cctype>
# include <algorithm>
# include <sstream>
# include <imgui/imgui.h>

ZDSJ::Command::Command()
{
	this->_context = ZDSJ::Context::getInstance();
	this->registeCommand("command_history", "clear command history", [&](std::string& _data) {
		if (_data == "clear") {
			this->m_history.clear();
			return true;
		}
		return false;
	});
	this->registeCommand("help", "show help", [&](std::string& _data) {
		if (_data.empty()) {
			this->help();
		}
		else {
			auto item = this->m_command_handlers.find(_data);
			if (item != this->m_command_handlers.end()) {
				ZDSJ::ringBuffer->push(item->second.first);
			}
			else {
				return false;
			}
		}
		return true;
	});
}

void ZDSJ::Command::execCommand(std::string& _command)
{
	if (std::all_of(_command.begin(), _command.end(), [](unsigned char _c) {
		return std::isspace(static_cast<int>(_c));
		})) {
	}
	ZDSJ::ringBuffer->push(_command);
	std::istringstream iss(_command);
	std::string operation;
	std::string data;
	std::getline(iss, operation, ':');
	std::getline(iss, data, ':');
	if (this->m_history.size() >= this->m_max_history) {
		this->m_history.erase(this->m_history.begin());
	}
	this->m_history.push_back(_command);
	this->m_history_pos = this->m_history.size();
	this->exec(operation, data);
}

ZDSJ::Command::~Command()
{
}

ZDSJ::Command* ZDSJ::Command::getInstance()
{
	static ZDSJ::Command _instance;
	return &_instance;
}

bool ZDSJ::Command::registeCommand(const std::string& _operation, const std::string& _description, HandlerFunc _handle_func)
{
	auto item = this->m_command_handlers.find(_operation);
	if (item != this->m_command_handlers.end()) {
		return false;
	}
	this->m_command_handlers.insert(std::make_pair(_operation, std::make_pair(_description, _handle_func)));
	return true;
}

int ZDSJ::Command::textEditCallback(ImGuiInputTextCallbackData* _data)
{
	switch (_data->EventFlag)
	{
	case ImGuiInputTextFlags_CallbackHistory:
		switch (_data->EventKey)
		{
		case ImGuiKey_UpArrow:
			this->m_history_pos = std::max(0, this->m_history_pos - 1);
			break;
		case ImGuiKey_DownArrow:
			this->m_history_pos += 1;
			if (this->m_history_pos > this->m_history.size() - 1) {
				this->m_history_pos = this->m_history.size() - 1;
			}
			break;
		}
		if (this->m_history_pos < this->m_history.size()) {
			_data->DeleteChars(0, _data->BufTextLen);
			_data->InsertChars(0, this->m_history.at(this->m_history_pos).data());
		}
		break;
	}
	return 0;
}

void ZDSJ::Command::exec(std::string& _operation, std::string& _data)
{
	std::istringstream iss(_data);
	auto item = this->m_command_handlers.find(_operation);
	if (item != this->m_command_handlers.end()) {
		if (!item->second.second(_data)) {
			ZDSJ::ringBuffer->push("error data");
		}
	}
}

void ZDSJ::Command::help()
{
	if (this->m_help.empty()) {
		std::ostringstream oss;
		oss << "---------\n";
		oss << "[operation]:[data] like help:animation_run to see animation_run description\n";
		for (auto item : this->m_command_handlers) {
			oss << "\t - " << item.first << ": " << item.second.first << "\n";
		}
		this->m_help = oss.str();
	}
	
	ZDSJ::ringBuffer->push(this->m_help);
}