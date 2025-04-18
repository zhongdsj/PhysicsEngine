# include <Physics/Collision.h>
# include <DirectXCollision.h>
# include <Context.h>
# include <DirectXMath.h>
# include <Physics/MovementInterface.h>
# include <MyDx11/DrawAbleData.h>

ZDSJ::Collision::Collision(MovementInterface* _movement, const DirectX::XMFLOAT3& _center, float _radius): PhysicsInterface(_movement),
	m_boundingSphere(std::make_shared<DirectX::BoundingSphere>(_center, _radius))
{
	_movement->getData()->connect("position", new Slot([&]()
	{
		this->m_boundingSphere->Center = this->getMovement()->getData()->position();
	}));
}

bool ZDSJ::Collision::intersects(const Collision* _other) const
{
	return this->m_boundingSphere->Intersects((*_other->m_boundingSphere.get()));
}

DirectX::XMFLOAT3 ZDSJ::Collision::calculateForce(PhysicsInterface* _other)
{
	auto other_collision = reinterpret_cast<Collision*>(_other);
	if(other_collision == nullptr)
	{
		return { 0.0f, 0.0f, 0.0f };
	}
	if(this->intersects(other_collision))
	{
		// 提取速度、质量、位置
		auto this_velocity = DirectX::XMLoadFloat3(this->getMovement()->getVelocity());
		auto this_mass = this->getMovement()->getMass();
		auto other_velocity = DirectX::XMLoadFloat3(other_collision->getMovement()->getVelocity());
		auto other_mass = other_collision->getMovement()->getMass();
		auto this_position = DirectX::XMLoadFloat3(&this->getMovement()->getData()->position());
		auto other_position = DirectX::XMLoadFloat3(&other_collision->getMovement()->getData()->position());
		// 计算碰撞方向（从 A 到 B 的单位向量）
		DirectX::XMVECTOR delta = DirectX::XMVectorSubtract(other_position, this_position);
		float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(delta));
		DirectX::XMVECTOR normal = DirectX::XMVectorDivide(delta, DirectX::XMVectorSet(distance, distance, distance, 1.0f)); // 单位向量

		// 计算相对速度
		DirectX::XMVECTOR relative_velocity = DirectX::XMVectorSubtract(this_velocity, other_velocity);
		float relative_velocity_dot_normal = DirectX::XMVectorGetX(DirectX::XMVector3Dot(relative_velocity, normal));

		// 修正位置（避免穿透）
		float penetration = (this->m_boundingSphere->Radius + other_collision->m_boundingSphere->Radius - distance) * 0.5f; // 平均穿透深度
		DirectX::XMFLOAT3 this_new_position;
		DirectX::XMFLOAT3 other_new_position;
		DirectX::XMStoreFloat3(&this_new_position, DirectX::XMVectorSubtract(this_position, DirectX::XMVectorScale(normal, penetration)));
		DirectX::XMStoreFloat3(&other_new_position, DirectX::XMVectorAdd(other_position, DirectX::XMVectorScale(normal, penetration)));
		this->getMovement()->getData()->position(this_new_position);
		other_collision->getMovement()->getData()->position(other_new_position);
		
		// 碰撞公式
		float this_impulse_factor = 2.0f * other_mass / (this_mass + other_mass);
		float other_impulse_factor = 2.0f * this_mass / (this_mass + other_mass);
		DirectX::XMVECTOR this_impulse = DirectX::XMVectorScale(normal, this_impulse_factor * relative_velocity_dot_normal);
		DirectX::XMVECTOR other_impulse = DirectX::XMVectorScale(normal, other_impulse_factor * relative_velocity_dot_normal);
		DirectX::XMFLOAT3 this_new_velocity;
		DirectX::XMFLOAT3 other_new_velocity;
		DirectX::XMStoreFloat3(&this_new_velocity, this_impulse);
		DirectX::XMStoreFloat3(&other_new_velocity, other_impulse);
		this->getMovement()->subVelocity(this_new_velocity);
		_other->getMovement()->addVelocity(other_new_velocity);
	}
	return {0.0f, 0.0f, 0.0f};
}

ZDSJ::Collision::~Collision()
{

}
