# pragma once
# include <functional>

namespace ZDSJ {
	class DrawAbleAdapter;

	class DrawAbleAnimation
	{
	public:
		DrawAbleAnimation(float _change_value, long long _animation_time, short _fps, std::function<void(DrawAbleAdapter*, float)> _update_function, std::function<float(float, float)> _exchange_function, bool _loop = false);
		// -method: update
		// -brief: 动画更新(逐帧)
		// -param: DrawAbleAdapter* _drawable 动画应用的对象
		// -param: bool _continue 是否继续动画，做动画中断，false表示动画结束
		void update(DrawAbleAdapter* _drawable, bool _continue);

		static ZDSJ::DrawAbleAnimation* rotationZAnimation(float _change_value, long long _amination_time, short _fps, bool _loop = false);

		~DrawAbleAnimation() = default;
	private:
		float m_change_value = 0.0f;
		float m_step = 0.0f;
		short m_animation_fps = 0;
		short m_time_base = 60;
		bool m_loop = false;
		std::function<void(DrawAbleAdapter*, float)> m_update_function;
		std::function<float(float, float)> m_exchange_function;
	};

}