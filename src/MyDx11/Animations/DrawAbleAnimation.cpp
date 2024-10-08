# include <MyDx11/Animations/DrawAbleAnimation.h>
# include <cmath>

ZDSJ::DrawAbleAnimation::DrawAbleAnimation(float _change_value, long long _animation_time, short _fps, 
	std::function<void(float)> _update_function, std::function<float(float, float)> _exchange_function, bool _loop):
	m_change_value(_change_value), m_time_base(_fps), m_update_function(_update_function), m_exchange_function(_exchange_function), m_loop(_loop)
{
	// ������ڵ�ǰʱ�����(֡��)����Ҫ����֡
	this->m_animation_fps = round((_fps / 1000.0f) * _animation_time);
}

void ZDSJ::DrawAbleAnimation::update(short _fps, bool _continue)
{
	// continue �������ж�
	
	// ����ʱ���
	float temp = this->m_time_base / _fps;
	temp /= this->m_animation_fps;
	this->m_step += temp;
	// TODO�任
	this->m_update_function(this->m_exchange_function(this->m_step, this->m_change_value));
	if (!this->m_loop) {
		if (this->m_step >= this->m_animation_fps) {
			// TODO �����ص�
		}
	}
	else if (!_continue) {
		// TODO �����ص�
	}
	
}
