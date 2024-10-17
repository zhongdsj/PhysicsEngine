# include <ImGuiManager/Command.h>
# include <RingBuffer.h>
# include <MyDx11/Context.h>
# include <cctype>
# include <algorithm>
# include <sstream>
# include <imgui/imgui.h>

ZDSJ::Command::Command(ZDSJ::Context* _context)
{
	this->registeCommand("command", "history", "clear command history", [&](std::string& _data) -> bool {
		if (_data.empty()) {
			this->m_history.clear();
			return true;
		}
		return false;
	});

	this->registeCommand("command", "clear", "clear command", [](std::string& _data) -> bool {
		if (_data.empty()) {
			ZDSJ::Context::getInstance()->ringBuffer()->clear();
			return true;
		}
		return false;
	});

	this->registeCommand("help", "", "show help", [&](std::string& _data) -> bool {
		if (_data.empty()) {
			this->help();
		}
		else {
			std::istringstream iss(_data);
			std::ostringstream oss;
			std::string category;
			std::string operation;
			std::getline(iss, category, ':');
			std::getline(iss, operation, ':');
			auto category_map = this->m_command_handlers.find(category);
			if (category_map != this->m_command_handlers.end()) {
				if (operation.empty()) {
					oss << "- " << category << "\n";
					for (auto item : category_map->second) {
						oss << "\t- " << item.first << ":" << item.second.first << "\n";
					}
				}
				else {
					auto item = category_map->second.find(operation);
					if (item != category_map->second.end()) {
						oss << category << "-" << operation << ":" << item->first << " " << item->second.first << "\n";
					}
				}
				this->write(oss.str());
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
	this->write(_command);
	std::istringstream iss(_command);
	std::string category;
	std::string operation;
	std::string data;
	std::getline(iss, category, ':');
	std::getline(iss, operation, ':');
	std::getline(iss, data, ':');
	if (this->m_history.size() >= this->m_max_history) {
		this->m_history.erase(this->m_history.begin());
	}
	this->m_history.push_back(_command);
	this->m_history_pos = this->m_history.size();
	this->exec(category, operation, data);
}

void ZDSJ::Command::write(const std::string& _message)
{
	ZDSJ::Context::getInstance()->ringBuffer()->push(_message);
}

ZDSJ::Command::~Command()
{
}

bool ZDSJ::Command::registeCommand(const std::string& _category, const std::string& _operation, const std::string& _description, HandlerFunc _handle_func)
{
	auto category_map = this->m_command_handlers.find(_category);
	if (category_map == this->m_command_handlers.end()) {
		this->m_command_handlers.insert(std::make_pair(_category, std::unordered_map<std::string, std::pair<std::string, HandlerFunc>>()));
	}
	category_map = this->m_command_handlers.find(_category);
	auto operation = category_map->second.find(_operation);
	if (operation != category_map->second.end()) {
		return false;
	}
	category_map->second.insert(std::make_pair(_operation, std::make_pair(_description, _handle_func)));
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

void ZDSJ::Command::exec(std::string& _category, std::string& _operation, std::string& _data)
{
	auto category_map = this->m_command_handlers.find(_category);
	if (category_map == this->m_command_handlers.end()) {
		// this->write("error category");
		return;
	}
	auto operation = category_map->second.find(_operation);
	if (_category == "help" && !_operation.empty()) {
		operation = category_map->second.find("");
		std::string data = _operation + ":" + _data;
		if (!operation->second.second(data)) {
			this->write("error data");
		}
		return;
	}
	if (operation == category_map->second.end()) {
		this->write("error operation");
		return;
	}
	if (!operation->second.second(_data)) {
		this->write("error data");
	}
}

void ZDSJ::Command::help()
{
	if (this->m_help.empty()) {
		std::ostringstream oss;
		oss << "---------\n";
		oss << "[category]:[operation]:[data] like help:animation_run to see animation_run description\n";
		for (auto category_map : this->m_command_handlers) {
			oss << "- " << category_map.first << "\n";
			for (auto operation : category_map.second) {
				oss << "\t- " << operation.first << ": " << operation.second.first << "\n";
			}
		}
		this->m_help = oss.str();
	}
	this->write(this->m_help);
}