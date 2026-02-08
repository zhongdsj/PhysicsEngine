# pragma once
# include <CommonExport.h>
# include <CommonMacro.h>
# include <shared_mutex>
# include <string>
# include <unordered_map>
# include <Slot.h>
# include <Command.h>

namespace ZDSJ
{
	class Common_Api CommandNode {
	public:
		friend class Command;
		CommandNode() = default;
		
		Delete_Copy_And_Move(CommandNode);
	private:
		mutable std::shared_mutex m_mutex;
		std::unordered_map<std::string, CommandNode*>* m_children = nullptr;
		const CommandNode* getChild(const std::string& name) const;
		ReadOnly(std::shared_ptr<Slot>, slot, nullptr, Slot)
	protected:
		void setSlot(Slot* slot);
		void addChildren(std::vector<std::string> path, Slot* slot);
	};
}
