# pragma once
# include <CommonExport.h>
# include <CommonMacro.h>
# include <atomic>
# include <KeyEnum.h>
# include <queue>
# define Keyboard_Instance ZDSJ::Keyboard::getInstance()

namespace ZDSJ
{
	enum class KeyOperation;
	class Common_Api Keyboard {
	public:
		static Keyboard* getInstance();
		Delete_Copy_And_Move(Keyboard);
		void setKeyState(SpecialKey key, KeyOperation is_pressed);
		void addAxisStates(AxisKey axis, const float delta);
		void frameUpdate();  // 帧尾处理
		void logicSyncEvent();
		void clearEvents();
		// void resetAllKeys();
		bool isKeyPressed(SpecialKey key) const;
		bool isKeyReleased(SpecialKey key) const;
		bool isKeyDown(SpecialKey key) const;
		float getAxisState(AxisKey axis) const;
		~Keyboard() = default;
	private:
		Keyboard();
		// 按键bitmap
		std::atomic<uint32_t> m_currKeyDown{ 0 }; // 当前渲染帧键位是否按住
		std::atomic<uint32_t> m_currKeyReleased{ 0 }; // 当前渲染帧键位是否抬起
		std::atomic<uint32_t> m_currKeyPressed{ 0 }; // 当前渲染帧键位是否按下
		// 轴数据
		std::atomic<float> m_axisStates[axis_key_count];

		// 按键事件队列
		std::queue<KeyEvent> m_keyEvents;

		// 轴事件队列
		std::queue<AxisEvent> m_axisEvents;
	};
}
