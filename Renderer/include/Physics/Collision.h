# pragma once
# include <memory>
# include <Physics/PhysicsInterface.h>

namespace DirectX
{
	class BoundingSphere;
}

namespace ZDSJ
{
	class Collision: public PhysicsInterface
	{
	public:
		// TODO 使用dx api创建视锥，定义物体向量
		// XMVector3Project方法
		Collision(MovementInterface* _movement, const DirectX::XMFLOAT3& _center, float _radius);
		DirectX::XMFLOAT3 calculateForce(PhysicsInterface* _other) override;
		~Collision();
	private:
		/**
		 * @brief 判断两个碰撞球是否碰撞
		 * @param _other
		 * @return true/false: 碰撞/未碰撞
		 */
		bool intersects(const Collision* _other) const;
		std::shared_ptr<DirectX::BoundingSphere> m_boundingSphere;
	};
}
