# pragma once
# include <vector>
# include <Physics/CollisionInterface.h>

namespace ZDSJ
{
	class Slot;
	class SphereCollision: public CollisionInterface
	{
	public:
		// TODO 使用dx api创建视锥，定义物体向量
		// XMVector3Project方法
		SphereCollision(MovementInterface* _movement, const DirectX::XMFLOAT3& _center, float _radius);
		bool intersects(CollisionInterface* _other) const override;
		bool intersects(const Ray& _ray) const override;
		DirectX::XMFLOAT3 calculateForce(PhysicsInterface* _other) override;

		~SphereCollision() override;

	protected:
		void movementChanged() override;
	private:
		const DirectX::BoundingSphere& boundingSphere() const;
		std::vector<Slot*> m_slots;
	};
}
