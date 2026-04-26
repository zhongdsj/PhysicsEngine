# include <Physics/SphereCollision.h>
# include <Context.h>
# include <DirectXCollision.h>
# include <DirectXMath.h>
# include <Physics/MovementInterface.h>
# include <MyDx11/DrawAbleData.h>

#include "MyDx11/Ray.h"

ZDSJ::SphereCollision::SphereCollision(MovementInterface* _movement, const DirectX::XMFLOAT3& _center, float _radius): CollisionInterface(_movement)
{
	this->m_bounding.push_back(std::make_shared<BoundingObject>(BoundingType::sphere, DirectX::BoundingSphere(_center, _radius)));
	this->m_slots.push_back(_movement->getData()->connect("position", new Slot([&]()
	{
		this->movementChanged();
	})));
}

bool ZDSJ::SphereCollision::intersects(CollisionInterface* _other) const
{
	bool result = false;
	for(auto& item: _other->getBounding())
	{
		switch (item->bounding_type)
		{
		case BoundingType::custom:
			result |= item->custom->intersects(this->boundingSphere());
			break;
		case BoundingType::sphere:
			result |= this->boundingSphere().Intersects(item->sphere);
			break;
		case BoundingType::box:
			result |= this->boundingSphere().Intersects(item->box);
			break;
		default: 
			break;
		}
		if(result)
		{
			break;
		}
	}
	return result;
}

bool ZDSJ::SphereCollision::intersects(const Ray& _ray) const
{
	float dist = _ray.dist();
	return this->boundingSphere().Intersects(_ray.origin(), _ray.direction(), dist);
}

DirectX::XMFLOAT3 ZDSJ::SphereCollision::calculateForce(PhysicsInterface* _other)
{
	auto other_collision = reinterpret_cast<SphereCollision*>(_other);
	DirectX::XMFLOAT3 defaultResult = { 0.0f, 0.0f, 0.0f };
	if(other_collision == nullptr)
	{
		return defaultResult;
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

		// 计算碰撞方向（从 A 到 B 的向量）

		float sum_radius = this->boundingSphere().Radius + other_collision->boundingSphere().Radius;
		DirectX::XMVECTOR delta = DirectX::XMVectorSubtract(other_position, this_position);
		float distance = DirectX::XMVectorGetX(DirectX::XMVector3Length(delta));
		DirectX::XMVECTOR normal;

		const float MIN_DIST = 0.001f;

		// 引擎标准：重合时不计算法线，避免 INF
		if (distance < MIN_DIST)
		{
			normal = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
			distance = MIN_DIST;
		}
		else
		{
			normal = DirectX::XMVector3Normalize(delta);
		}

		// 相对速度
		DirectX::XMVECTOR relative_velocity = DirectX::XMVectorSubtract(this_velocity, other_velocity);
		float vel_dot = DirectX::XMVectorGetX(DirectX::XMVector3Dot(relative_velocity, normal));

		// 只有物体相互靠近时才处理碰撞（防止反复震荡）
		if (vel_dot > 0.0f)
			return { 0.0f, 0.0f, 0.0f};

		// ==============================
		// 位置修正（解决穿透）
		// ==============================
		float penetration = (sum_radius - distance) * 0.5f;
		if (penetration < 0) penetration = 0;

		DirectX::XMFLOAT3 this_new_pos;
		DirectX::XMFLOAT3 other_new_pos;
		DirectX::XMStoreFloat3(&this_new_pos, DirectX::XMVectorSubtract(this_position, DirectX::XMVectorScale(normal, penetration)));
		DirectX::XMStoreFloat3(&other_new_pos, DirectX::XMVectorAdd(other_position, DirectX::XMVectorScale(normal, penetration)));
		this->getMovement()->getData()->position(this_new_pos);
		other_collision->getMovement()->getData()->position(other_new_pos);

		float restitution = 0.8f; // 弹性系数 0~1
		float impulse_j = -(1 + restitution) * vel_dot;
		float total_mass = this_mass + other_mass;

		// 质量为0的特殊情况（静态物体）
		if (total_mass < 0.0001f) total_mass = 0.0001f;
		impulse_j /= total_mass;

		DirectX::XMVECTOR impulse = DirectX::XMVectorScale(normal, impulse_j);

		DirectX::XMFLOAT3 v1;
		DirectX::XMFLOAT3 v2;
		DirectX::XMStoreFloat3(&v1, DirectX::XMVectorScale(impulse, other_mass));
		DirectX::XMStoreFloat3(&v2, DirectX::XMVectorScale(impulse, this_mass));

		this->getMovement()->subVelocity(v1);
		other_collision->getMovement()->addVelocity(v2);
	}
	return defaultResult;
}

ZDSJ::SphereCollision::~SphereCollision()
{
	for(auto& item: this->m_slots)
	{
		item->useful(false);
	}
}

void ZDSJ::SphereCollision::movementChanged()
{
	for (auto& item : this->m_bounding)
	{
		switch (item->bounding_type)
		{
		case BoundingType::custom:
			item->custom->movementChanged(this->getMovement()->getData()->size(), this->getMovement()->getData()->position(), this->getMovement()->getData()->rotation());
			break;
		case BoundingType::sphere:
			item->sphere.Center = this->getMovement()->getData()->position();
			break;
		case BoundingType::box:
			item->box.Center = this->getMovement()->getData()->position();
			break;
		}
	}
}

const DirectX::BoundingSphere& ZDSJ::SphereCollision::boundingSphere() const
{
	return this->m_bounding.at(0)->sphere;
}
