# pragma once
# include <MyDx11/Camera.h>
# include <RingBuffer.h>
# include <ImGuiManager/Command.h>
# include <MyDx11/Keyboard.h>

namespace ZDSJ {

# define fields(type, fields_name, default_value, func_name) type m_##fields_name = default_value; \
	public: inline Context* func_name(type _value){ this->m_##fields_name = _value; return this; } \
	inline type func_name() const { return this->m_##fields_name; } \
	private: \

# define ReadOnly(type, fields_name, default_value, func_name) type m_##fields_name = default_value; \
	public: inline type func_name() const { return this->m_##fields_name; } \
	private: \

# define GetPtr(type, fields_name, default_value, func_name) type m_##fields_name = default_value; \
	public: inline type func_name() { return this->m_##fields_name; } \
	private: \

	class Context {
	public:
		static Context* getInstance();

		inline Context* windowWidth(float _value) {
			this->m_window_width = _value;
			this->m_window_rate = this->m_window_width / this->m_window_height;
			return this;
		}

		inline Context* windowHeight(float _value) {
			this->m_window_height = _value;
			this->m_window_rate = this->m_window_width / this->m_window_height;
			return this;
		}

	private:
		fields(float, fps, 0.0f, fps)
		fields(float, use_time, 0.0f, useTime)
		fields(bool, animation_run, true, animationRun)
		ReadOnly(float, window_width, 800.0f, windowWidth)
		ReadOnly(float, window_height, 600.0f, windowHeight)
		ReadOnly(float, window_rate, 800.0f / 600.0f, windowRate)
		
		GetPtr(Camera*, camera, nullptr, camera)
		GetPtr(RingBuffer<std::string>*, ring_buffer, nullptr, ringBuffer)
		GetPtr(Command*, command, nullptr, command)
		GetPtr(Keyboard*, keyboard, nullptr, Keyboard);
		Context();
	protected:
		~Context();
	};

}