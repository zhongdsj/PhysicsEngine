# pragma once

namespace DirectX
{
	struct XMFLOAT3;

}

namespace ZDSJ
{
	struct Vector;
	class DrawAbleInterface;

	class MovementInterface
	{

	public:
		virtual void calculateForce(MovementInterface* _other) = 0;
		virtual DirectX::XMFLOAT3 calculateAcceleration() = 0;
		virtual float getQuality() const = 0;
		virtual DrawAbleInterface* getDrawAble() const = 0;
		virtual ~MovementInterface() = default;
	};

}
