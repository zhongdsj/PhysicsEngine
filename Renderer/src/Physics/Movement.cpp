# include <Physics/Movement.h>
# include <Physics/SphereCollision.h>
# include <Physics/PhysicsInterface.h>
# include <MyDx11/DrawAbleData.h>

ZDSJ::Movement::Movement(DrawAbleData* _data, float _mass): m_mass(_mass), m_velocity(std::make_shared<DirectX::XMFLOAT3>(0.0f, 0.0f, 0.0f))
                                                            , m_linearAcceleration(std::make_shared<DirectX::XMFLOAT3>(0.0f, 0.0f, 0.0f)), m_data(_data)
{
	this->m_collision = std::make_shared<SphereCollision>(this, _data->position(), 10.0f);
}

void ZDSJ::Movement::applyForce(MovementInterface* _other)
{
	// 先计算碰撞
	if(_other->getCollision() != nullptr && this->m_collision != nullptr)
	{
		this->m_collision->calculateForce(_other->getCollision());
	}
	DirectX::XMVECTOR force = DirectX::XMVectorZero();;
	for(auto pair: this->m_physics)
	{
		auto physics_interface = _other->getPhysicsByType(pair.first);
		if(physics_interface != nullptr)
		{
			// 计算受力
			DirectX::XMFLOAT3 result = physics_interface->calculateForce(pair.second.get());
			force = DirectX::XMVectorAdd(force, XMLoadFloat3(&result));
		}
	}
	// 通过施加的力计算加速度
	float inv_mass = 1.0f / this->m_mass;
	// a = f/m
	DirectX::XMVECTOR acceleration = DirectX::XMVectorScale(force, inv_mass);
	// this->m_linearAcceleration += acceleration;
	DirectX::XMStoreFloat3(this->m_linearAcceleration.get(), acceleration);
}

void ZDSJ::Movement::updatePosition(const DirectX::XMFLOAT3& _position, float _magnification)
{
	// 半隐式欧拉积分
	DirectX::XMVECTOR velocity = DirectX::XMLoadFloat3(this->m_velocity.get());
	DirectX::XMVECTOR acceleration = DirectX::XMLoadFloat3(this->m_linearAcceleration.get());
	velocity = DirectX::XMVectorAdd(velocity, DirectX::XMVectorScale(acceleration, _magnification));
	DirectX::XMStoreFloat3(this->m_velocity.get(), velocity);

	// 更新位置
	DirectX::XMFLOAT3 position;
	DirectX::XMStoreFloat3(&position, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&_position), DirectX::XMVectorScale(velocity, _magnification)));
	this->m_data->position(position);
}

ZDSJ::PhysicsInterface* ZDSJ::Movement::getPhysicsByType(std::type_index _type_index) const
{
	auto iterator = this->m_physics.find(_type_index);
	if(iterator != this->m_physics.end())
	{
		return iterator->second.get();
	}
	return nullptr;
}

const DirectX::XMFLOAT3* ZDSJ::Movement::getVelocity() const
{
	return this->m_velocity.get();
}

void ZDSJ::Movement::addVelocity(const DirectX::XMFLOAT3& _velocity) const
{
	this->m_velocity->x += _velocity.x;
	this->m_velocity->y += _velocity.y;
	this->m_velocity->z += _velocity.z;
}

void ZDSJ::Movement::subVelocity(const DirectX::XMFLOAT3& _velocity) const
{
	this->m_velocity->x -= _velocity.x;
	this->m_velocity->y -= _velocity.y;
	this->m_velocity->z -= _velocity.z;
}

float ZDSJ::Movement::getMass() const
{
	return this->m_mass;
}

ZDSJ::DrawAbleData* ZDSJ::Movement::getData()
{
	return this->m_data;
}


void ZDSJ::Movement::addPhysics(std::type_index _type_index, const std::shared_ptr<PhysicsInterface>& _ptr)
{
	this->m_physics.insert_or_assign(_type_index, _ptr);
}

ZDSJ::CollisionInterface* ZDSJ::Movement::getCollision() const
{
	return this->m_collision.get();
}
