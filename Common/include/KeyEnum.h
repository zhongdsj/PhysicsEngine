# pragma once

namespace ZDSJ
{
	enum class KeyOperation
	{
		up = 0,
		down = 1,
	};

	enum class SpecialKey : uint8_t {
		key_wavy,    // 波浪键
		key_ctrl,    // Ctrl键
		key_shift,   // Shift键
		key_alt,     // Alt键
		key_space,   // 空格键
		key_esc,     // ESC键
		key_tab,     // Tab键
		key_caps,    // 大小写锁定
		mouse_left,  // 鼠标左键
		max_keys     // 用于标记枚举上限，不可作为按键使用
	};

	enum class AxisKey: uint8_t {
		mouse_wheel, // 滚轮
		max_keys     // 用于标记枚举上限，不可作为按键使用
	};

	struct KeyEvent
	{
		SpecialKey key;
		KeyOperation pressed; // true=按下, false=抬起

		KeyEvent(SpecialKey k, KeyOperation p) : key(k), pressed(p) {}
	};

	struct AxisEvent
	{
		AxisKey axis;
		float delta;

		AxisEvent(AxisKey k, float dt) : axis(k), delta(dt) {}
	};

	constexpr size_t axis_key_count = static_cast<size_t>(AxisKey::max_keys);
}
