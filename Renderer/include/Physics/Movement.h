# pragma once
#include <map>
# include <memory>
# include <Physics/MovementInterface.h>

namespace ZDSJ
{
	class DrawAbleData;
	class Movement: public MovementInterface
	{
	public:
		Movement(DrawAbleData* _data, float _mass);
		/**
		 * @brief 计算两个物体之间的力
		 * @param _other 
		 */
		void applyForce(MovementInterface* _other) override;
		/**
		 * @brief 更新下一个时间片的位置
		 * @param _position 当前位置引用，将会在计算后更改
		 * @param _magnification 时间片步长
		 */
		void updatePosition(const DirectX::XMFLOAT3& _position, float _magnification) override;
		PhysicsInterface* getPhysicsByType(std::type_index _type_index) const override;
		const DirectX::XMFLOAT3* getVelocity() const override;
		void addVelocity(const DirectX::XMFLOAT3& _velocity) const override;
		void subVelocity(const DirectX::XMFLOAT3& _velocity) const override;
		float getMass() const override;
		Collision* getCollision() override;
		DrawAbleData* getData() override;
		void addPhysics(std::type_index _type_index, const std::shared_ptr<PhysicsInterface>& _ptr);
		~Movement() override = default;
	private:
		// 质量
		float m_mass = 1.0f;
		// 速度
		std::shared_ptr<DirectX::XMFLOAT3> m_velocity;
		// 线性加速度
		std::shared_ptr<DirectX::XMFLOAT3> m_linearAcceleration;
		// 物理量
		std::map<std::type_index, std::shared_ptr<PhysicsInterface>> m_physics;
		// 碰撞
		std::shared_ptr<Collision> m_collision = nullptr;
		// 数据
		DrawAbleData* m_data = nullptr;
	};
}
