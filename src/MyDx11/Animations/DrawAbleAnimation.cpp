# include <MyDx11/Animations/DrawAbleAnimation.h>
# include <MyDx11/DrawAble/DrawAbleAdapter.h>
# include <DirectXMath.h>
# include <cmath>
# include <Context/FpsContext.h>

ZDSJ::DrawAbleAnimation::DrawAbleAnimation(float _change_value, long long _animation_time, short _fps, 
	std::function<void(DrawAbleAdapter*, float)> _update_function, std::function<float(float, float)> _exchange_function, bool _loop):
	m_change_value(_change_value), m_time_base(_fps), m_update_function(_update_function), m_exchange_function(_exchange_function), m_loop(_loop)
{
	// 计算出在当前时间基下(帧数)下需要多少帧
	this->m_animation_fps = round((_fps / 1000.0f) * _animation_time);
}

void ZDSJ::DrawAbleAnimation::update(DrawAbleAdapter* _drawable, bool _continue)
{
	// continue 做动画中断
	ZDSJ_FpsContext_Init_Assert;
	// 换算时间基
	float temp = this->m_time_base / ZDSJ::FpsContext::fps_context->fps() / this->m_animation_fps;

	this->m_step += temp;
	this->m_update_function(_drawable, this->m_exchange_function(this->m_step, this->m_change_value));
	if (!this->m_loop) {
		if (this->m_step >= this->m_animation_fps) {
			// TODO 结束回调
		}
	}
	else if (!_continue) {
		// TODO 结束回调
	}
	
}

ZDSJ::DrawAbleAnimation* ZDSJ::DrawAbleAnimation::rotationZAnimation(float _change_value, long long _amination_time, short _fps, bool _loop)
{
	return new ZDSJ::DrawAbleAnimation((_change_value / 180.0f) * DirectX::XM_PI, _amination_time, _fps, [](DrawAbleAdapter* _drawable, float _value) {
		_drawable->setRotationZ(_value);
		}, [](float _step, float _change)->float {
			return _step * _change;
		});
}
