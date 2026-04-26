# include <Keyboard.h>
# include <Context.h>
# include <Slot.h>

ZDSJ::Keyboard* ZDSJ::Keyboard::getInstance()
{
	static ZDSJ::Keyboard instance;
	return &instance;
}

void ZDSJ::Keyboard::setKeyState(SpecialKey key, KeyOperation is_pressed)
{
	if (key >= SpecialKey::max_keys) return; // 非法按键直接返回

	// 位运算更新状态：原子操作保证多线程安全
	uint32_t mask = 1 << static_cast<uint8_t>(key);
	if (is_pressed == KeyOperation::down) {
		this->m_currKeyStates.fetch_or(mask, std::memory_order_relaxed);
	}
	else {
		this->m_currKeyStates.fetch_and(~mask, std::memory_order_relaxed);
	}

	// 数据放入队列
	this->m_keyEvents.emplace(key, is_pressed);
}

void ZDSJ::Keyboard::addAxisStates(AxisKey axis, const float delta)
{
	if (axis >= AxisKey::max_keys) return;

	const auto idx = static_cast<size_t>(axis);
	float expected = this->m_axisStates[idx].load(std::memory_order_relaxed);
	for (;;)
	{
		const float desired = expected + delta;
		if (this->m_axisStates[idx].compare_exchange_weak(expected, desired, std::memory_order_relaxed, std::memory_order_relaxed))
			break;
	}
	this->m_axisEvents.emplace(axis, delta);
}

void ZDSJ::Keyboard::frameUpdate()
{
	// 清空轴缓存
	memset(this->m_axisStates, 0.0f, sizeof(this->m_axisStates));

	// --- 按键：保存上一帧状态 ---
	this->m_prevKeyStates.store(
		this->m_currKeyStates.load(std::memory_order_relaxed),
		std::memory_order_relaxed
	);
}

void ZDSJ::Keyboard::logicSyncEvent()
{
	// todo 处理队列

	// todo 正常不需要清空
	this->clearEvents();
}

void ZDSJ::Keyboard::clearEvents()
{
	// 清空队列
	std::queue<KeyEvent>().swap(this->m_keyEvents);
	std::queue<AxisEvent>().swap(this->m_axisEvents);
}

bool ZDSJ::Keyboard::isKeyPressed(SpecialKey key) const
{
	if (key >= SpecialKey::max_keys) return false;
	uint32_t mask = 1u << static_cast<uint8_t>(key);
	return (m_currKeyStates & mask) != 0;
}

bool ZDSJ::Keyboard::isKeyReleased(SpecialKey key) const
{
	if (key >= SpecialKey::max_keys) return false;
	uint32_t mask = 1u << static_cast<uint8_t>(key);
	bool curr = (m_currKeyStates & mask) != 0;
	bool prev = (m_prevKeyStates & mask) != 0;
	return !curr && prev;
}

float ZDSJ::Keyboard::getAxisState(AxisKey axis) const
{
	if (axis >= AxisKey::max_keys) return 0.0f;
	const auto idx = static_cast<size_t>(axis);
	return this->m_axisStates[idx].load(std::memory_order_relaxed);
}

ZDSJ::Keyboard::Keyboard()
{
	this->m_currKeyStates.store(0);
	memset(this->m_axisStates, 0.0f, sizeof(this->m_axisStates));
}