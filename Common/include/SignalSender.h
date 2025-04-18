# pragma once
# include <CommonExport.h>
# include <string>

namespace ZDSJ
{
	class Slot;

	class Common_Api SignalSender
	{
	public:
		virtual ~SignalSender() = default;
		virtual Slot* connect(const std::string& signal, Slot* slot) = 0;

		template <class... Args>
		void emit(const std::string& signal, Args&&... args) {};
	protected:
		virtual void registerSignal() = 0;
	};
}
