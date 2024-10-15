# include <MyDx11/Animations/DrawAbleAnimation.h>
# include <MyDx11/DrawAble/DrawAbleAdapter.h>
# include <DirectXMath.h>
# include <cmath>
# include <MyDx11/Context.h>
# include <ImGuiManager/Command.h>

ZDSJ::DrawAbleAnimation::DrawAbleAnimation(float _change_value, long long _animation_time, short _fps, 
	std::function<void(DrawAbleAdapter*, float)> _update_function, std::function<float(float, float)> _exchange_function, bool _loop):
	m_change_value(_change_value), m_time_base(_fps), m_update_function(_update_function), m_exchange_function(_exchange_function), m_loop(_loop)
{
	// ������ڵ�ǰʱ�����(֡��)����Ҫ����֡
	this->m_animation_fps = round((_fps / 1000.0f) * _animation_time);

	// ע��command
	ZDSJ::Command* command = ZDSJ::Command::getInstance();
	command->registeCommand("animation_run", "stop animation if data is false, continue with true", [&](std::string& _data) {
		if (_data == "true") {
			ZDSJ::Context::getInstance()->animationRun(true);
		}
		else if(_data == "false") {
			ZDSJ::Context::getInstance()->animationRun(false);
		}
		else {
			return false;
		}

		return true;
	});
}


void ZDSJ::DrawAbleAnimation::update(DrawAbleAdapter* _drawable, bool _continue)
{
	if (!ZDSJ::Context::getInstance()->animationRun()) {
		// ��ͣ����
		return;
	}
	// ����ʱ���
	float temp = this->m_time_base / ZDSJ::Context::getInstance()->fps() / this->m_animation_fps;

	this->m_step += temp;
	this->m_update_function(_drawable, this->m_exchange_function(this->m_step, this->m_change_value));
	if (!this->m_loop) {
		if (this->m_step >= this->m_animation_fps) {
			// TODO �����ص�
		}
	}
	else if (!_continue) {
		// TODO �����ص�
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
