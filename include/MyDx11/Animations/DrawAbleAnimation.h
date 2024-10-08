# pragma once
# include <functional>

namespace ZDSJ {

	class DrawAbleAnimation
	{
	public:
		DrawAbleAnimation(float _change_value, long long _animation_time, short _fps, std::function<void(float)> _update_function, std::function<float(float, float)> _exchange_function, bool _loop = false);
		void update(short _fps, bool _continue);
		~DrawAbleAnimation() = default;
	private:
		float m_change_value = 0.0f;
		float m_step = 0.0f;
		short m_animation_fps = 0;
		short m_time_base = 60;
		bool m_loop = false;
		std::function<void(float)> m_update_function;
		std::function<float(float, float)> m_exchange_function;
	};

}