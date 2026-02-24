# include <Keyboard.h>
# include <KeyEnum.h>
# include <Context.h>
# include <ApplicationWindowInterface.h>
# include <Slot.h>

ZDSJ::Keyboard* ZDSJ::Keyboard::getInstance()
{
	static ZDSJ::Keyboard instance;
	instance.init();
	return &instance;
}

void ZDSJ::Keyboard::setKeyState(SpecialKey key, KeyOperation is_pressed)
{
	if (key >= SpecialKey::max_keys) return; // 非法按键直接返回

	// 位运算更新状态：原子操作保证多线程安全
	uint32_t mask = 1 << static_cast<uint8_t>(key);
	if (is_pressed == KeyOperation::down) {
		this->m_keyStates.fetch_or(mask, std::memory_order_relaxed);
	}
	else {
		this->m_keyStates.fetch_and(~mask, std::memory_order_relaxed);
	}
}

void ZDSJ::Keyboard::resetAllKeys()
{
	this->m_keyStates.store(0, std::memory_order_relaxed);
}

bool ZDSJ::Keyboard::isKeyPressed(SpecialKey key) const
{
	if (key >= SpecialKey::max_keys) return false;
	uint32_t mask = 1 << static_cast<uint8_t>(key);
	return (this->m_keyStates.load(std::memory_order_relaxed) & mask) != 0;
}

ZDSJ::Keyboard::Keyboard()
{
	this->m_keyStates.store(0);
}

void ZDSJ::Keyboard::init()
{
	Context_Instance->getWindow()->connect(ZDSJ::KEY::ctrl, new Slot([this](KeyOperation _operation)
	{
		this->setKeyState(SpecialKey::key_ctrl, _operation);
	}, "ctrl按下/弹起触发"));
}
