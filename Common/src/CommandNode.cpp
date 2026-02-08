# include <CommandNode.h>
# include <Context.h>

void ZDSJ::CommandNode::addChildren(std::vector<std::string> path, Slot* slot)
{
	std::unique_lock<std::shared_mutex> lock(this->m_mutex);
	CommandNode* current = this;
	for (size_t i = 0; i < path.size() - 1; ++i) {
		if (this->m_children == nullptr)
		{
			this->m_children = new std::unordered_map<std::string, CommandNode*>();
		}
		const auto& part = path[i];
		// 若子节点不存在，创建分类节点
		if (!current->m_children->count(part)) {
			current->m_children->emplace(part, new CommandNode());
		}
		current = current->m_children->at(part);
	}
	// 处理最后一个节点：命令节点
	const auto& cmd_name = path.back();
	if (current->m_children != nullptr && current->m_children->count(cmd_name)) {
		Log_Error("命令{}已存在", cmd_name);
		return;
	}
	CommandNode* last_mode = new CommandNode();
	last_mode->setSlot(slot);
	if (current->m_children == nullptr)
	{
		current->m_children = new std::unordered_map<std::string, CommandNode*>();
	}
	current->m_children->emplace(cmd_name, last_mode);
}

const ZDSJ::CommandNode* ZDSJ::CommandNode::getChild(const std::string& name) const
{
	if(this->m_children == nullptr)
	{
		return nullptr;
	}
	if(!this->m_children->count(name))
	{
		return nullptr;
	}
	return this->m_children->at(name);
}

void ZDSJ::CommandNode::setSlot(Slot* slot)
{
	this->m_slot = std::shared_ptr<Slot>(slot);
}
