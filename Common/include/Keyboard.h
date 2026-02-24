# pragma once
# include <CommonExport.h>
# include <CommonMacro.h>
# include <atomic>

namespace ZDSJ
{
	enum class SpecialKey : uint8_t;
	enum class KeyOperation;
	class Common_Api Keyboard {
	public:
		static Keyboard* getInstance();
		Delete_Copy_And_Move(Keyboard);
		void setKeyState(SpecialKey key, KeyOperation is_pressed);
		void resetAllKeys();
		bool isKeyPressed(SpecialKey key) const;
		~Keyboard() = default;
	private:
		Keyboard();
		void init();
		std::atomic<uint32_t> m_keyStates;
	};
}
