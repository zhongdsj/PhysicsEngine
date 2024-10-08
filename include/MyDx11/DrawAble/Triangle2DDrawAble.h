# pragma once
# include <MyDx11/DrawAble/DrawAbleAdapter.h>
# include <MyDx11/Animations/DrawAbleAnimation.h>
# include <vector>

struct ID3D11Buffer;

namespace ZDSJ {
	class BindAbleInterface;

	class Triangle2DDrawAble : public DrawAbleAdapter {
	public:
		Triangle2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context, float _translation_x = 0.0f, bool _change = false);
		void addAnimation(ZDSJ::DrawAbleAnimation* _animation);
		ZDSJ::DrawAbleAnimation* rotationZAnimation(float _change_value, long long _amination_time, short _fps, bool _loop=false);
		void setRotationZ(float _value);
		~Triangle2DDrawAble();
	protected:
		void update(ID3D11DeviceContext* _context, short _fps) override;
	private:
		ID3D11Buffer* m_buffer = nullptr;
		bool m_change = false;
		short m_run_fps = 0;
		short m_max_fps = 4.0f * 60;
		std::vector<ZDSJ::DrawAbleAnimation*> m_animation;
	};
}