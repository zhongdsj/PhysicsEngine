# include <Command.h>
# include <Context.h>
# include <CommandNode.h>
# include <queue>

ZDSJ::Command& ZDSJ::Command::getInstance()
{
	static Command instance;
	return instance;
}

ZDSJ::Slot* ZDSJ::Command::connect(const std::string& signal, Slot* slot)
{
	CommandNode* node = nullptr;
	auto path = splitPath(signal, ':');
	if (path.empty()) {
		Log_Error("命令路径为空");
		return nullptr;
	}

	if (!this->m_slots.count(path.at(0)))
	{
		node = new CommandNode();
		this->m_slots.emplace(path.at(0), node);
	}
	else
	{
		node = this->m_slots.at(path.at(0));
	}
	if (path.size() == 1)
	{
		node->setSlot(slot);
		return slot;
	}
	path.erase(path.begin());
	node->addChildren(path, slot);
	return slot;
}

void ZDSJ::Command::execCommand(const std::string& _command) const
{
	auto path_param = splitPath(_command, '-');
	if (path_param.empty())
	{
		// 命令为空  不处理
		return;
	}
	auto path = splitPath(path_param.at(0), ':');
	if (path.empty()) {
		Log_Error("命令路径为空");
		return;
	}
	// 获取入口
	auto begin = path.at(0);
	if (!this->m_slots.count(begin))
	{
		// 未获取到入口
		Log_Error("未获取到:{},注册的命令", begin);
		return;
	}
	const CommandNode* node = this->m_slots.at(begin);
	for (int i = 1; i < path.size(); ++i)
	{
		auto temp_cmd = path.at(i);
		node = node->getChild(temp_cmd);
		if (node == nullptr)
		{
			Log_Error("未获取到:{}, 注册的命令:{}", begin, temp_cmd);
			return;
		}
	}
	auto cmd = path.back();
	if (node->m_slot == nullptr)
	{
		Log_Error("命令:{}, 未设置方法", cmd);
		return;
	}
	if (!node->m_slot->useful())
	{
		Log_Error("命令:{}, 方法已失效", cmd);
		return;
	}
	if (path_param.size() != 1)
	{
		node->m_slot->emit(path_param.at(1));
		// 有参数;
	}
	else
	{
		node->m_slot->emit();
	}
}

ZDSJ::Command::Command()
{
	this->Command::registerSignal();
}

void ZDSJ::Command::registerSignal()
{
	this->connect("help", new Slot([this]
	{
		std::stringstream ss;
		ss << "\n";
		std::queue<std::tuple<const CommandNode*, const std::string, int>> node_queue;
		for (const auto slot : this->m_slots)
		{
			node_queue.emplace(std::make_tuple(slot.second, slot.first, 0));
		}
		while(!node_queue.empty())
		{
			auto poll = node_queue.front();
			node_queue.pop();
			const CommandNode* temp = std::get<0>(poll);
			int level = std::get<2>(poll);
			if(temp == nullptr)
			{
				continue;
			}
			ss << std::string(4 * level, ' ');
			ss << "- " << std::get<1>(poll) << ": " << temp->getSlot()->description() << "\n";
			if(temp->m_children == nullptr)
			{
				continue;
			}
			auto iterator = temp->m_children->begin();
			while (iterator != temp->m_children->end())
			{
				node_queue.emplace(std::make_tuple(iterator->second, iterator->first, level + 1));
				++iterator;
			}
		}
		Log_Info(ss.str());
	}, "查看命令描述"));
}
