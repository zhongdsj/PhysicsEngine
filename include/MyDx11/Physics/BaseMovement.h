# pragma once
# include <MyDx11/Physics/MovementInterface.h>

namespace ZDSJ
{
	class BaseMovement : public MovementInterface
	{
	public:
		BaseMovement(DrawAbleInterface* _drawable);

		void calculateForce(MovementInterface* _other) override;
		virtual DirectX::XMFLOAT3 calculateAcceleration() override;
		float getQuality() const override;
		DrawAbleInterface* getDrawAble() const;
		virtual ~BaseMovement() override;
	private:
		float m_quality = 1.0f;
		DrawAbleInterface* m_drawable = nullptr;
		DirectX::XMFLOAT3* m_f = nullptr;
		DirectX::XMFLOAT3* m_p = nullptr;
	};

}
