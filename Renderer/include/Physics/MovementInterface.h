# pragma once
# include <typeindex>

namespace DirectX
{
	class XMFLOAT3;
}

namespace ZDSJ
{
	class PhysicsInterface;
	class DrawAbleData;
	class Collision;
	class MovementInterface
	{
	public:
		virtual void applyForce(MovementInterface* _other) = 0;
		virtual void updatePosition(const DirectX::XMFLOAT3& _position, float _magnification) = 0;
		virtual PhysicsInterface* getPhysicsByType(std::type_index _type_index) const = 0;
		virtual const DirectX::XMFLOAT3* getVelocity() const = 0;
		virtual void addVelocity(const DirectX::XMFLOAT3& _velocity) const = 0;
		virtual void subVelocity(const DirectX::XMFLOAT3& _velocity) const = 0;
		virtual float getMass() const = 0;
		virtual Collision* getCollision() = 0;
		virtual DrawAbleData* getData() = 0;
		virtual ~MovementInterface() = default;
	};
}
