# pragma once

namespace DirectX
{
	class XMFLOAT3;
}

namespace ZDSJ
{
	class MovementInterface;
	class PhysicsInterface
	{
	public:
		PhysicsInterface(MovementInterface* _movement) : m_movement(_movement) {}
		virtual DirectX::XMFLOAT3 calculateForce(PhysicsInterface* _other) = 0;
		MovementInterface* getMovement() const { return this->m_movement; }
		virtual ~PhysicsInterface() = default;
	private:
		MovementInterface* m_movement = nullptr;
	};
}
