# pragma once

namespace ZDSJ
{
	namespace KEY
	{
		// 波浪键
		auto wavy = "wavy";
		// ctrl
		auto ctrl = "ctrl";
		// alt
		auto alt = "alt";
	}

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
		max_keys     // 用于标记枚举上限，不可作为按键使用
	};
}
