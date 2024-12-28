# include <MyDx11/Physics/BaseMovement.h>
# include <DirectXMath.h>
#include "MyDx11/DrawAble/DrawAbleInterface.h"

ZDSJ::BaseMovement::BaseMovement(DrawAbleInterface* _drawable)
{
	this->m_drawable = _drawable;
	this->m_f = new DirectX::XMFLOAT3(0, 0, 0);
	this->m_p = new DirectX::XMFLOAT3(0, 0, 0);
	this->m_quality = this->m_drawable->size().x;
}

void ZDSJ::BaseMovement::calculateForce(MovementInterface* _other)
{
	float other_quality = _other->getQuality();
	// 引力常数
	float G = 6.67 * 1e-1;
	
	const ZDSJ::float3& pos = this->m_drawable->position();
	const ZDSJ::float3& other_pos = _other->getDrawAble()->position();
	DirectX::XMFLOAT3 vector_3 = { other_pos.x - pos.x, other_pos.y - pos.y, other_pos.z - pos.z };
	DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&vector_3);
	DirectX::XMVECTOR vec_normal = DirectX::XMVector3Normalize(vec);
	const float r = DirectX::XMVectorGetX(DirectX::XMVector3Length(vec));
	if(r < (this->m_drawable->size().x / 2 + _other->getDrawAble()->size().x / 2))
	{
		return;
	}
	float f = (G * this->m_quality * other_quality) / (r*r);
	auto F = DirectX::XMVectorScale(vec_normal, f);
	DirectX::XMStoreFloat3(this->m_f, DirectX::XMVectorAdd(F, DirectX::XMLoadFloat3(this->m_f)));
}

DirectX::XMFLOAT3 ZDSJ::BaseMovement::calculateAcceleration()
{
	DirectX::XMFLOAT3 a;
	DirectX::XMStoreFloat3(&a, DirectX::XMVectorScale(DirectX::XMLoadFloat3(this->m_f), 1 / this->m_quality));
	this->m_f->x = 0;
	this->m_f->y = 0;
	this->m_f->z = 0;

	DirectX::XMStoreFloat3(this->m_p, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(this->m_p), DirectX::XMLoadFloat3(&a)));
	return *this->m_p;
}

float ZDSJ::BaseMovement::getQuality() const
{
	return this->m_quality;
}

ZDSJ::DrawAbleInterface* ZDSJ::BaseMovement::getDrawAble() const
{
	return this->m_drawable;
}

ZDSJ::BaseMovement::~BaseMovement()
{
	this->m_drawable = nullptr;
	delete this->m_f;
	delete this->m_p;
}