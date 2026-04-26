# include <MyDx11/DrawAbleData.h>
# include <Physics/Movement.h>

ZDSJ::DrawAbleData::DrawAbleData(const std::string& _render_type, const DirectX::XMFLOAT3& _position,
                                 const DirectX::XMFLOAT3& _size, const DirectX::XMFLOAT3& _rotation) : m_render_type(_render_type),
	m_size(_size),
	m_position(_position),
	m_rotation(_rotation)
{
	this->DrawAbleData::registerSignal();
	this->m_movement = std::make_shared<Movement>(this, 1.0f);
}

ZDSJ::Slot* ZDSJ::DrawAbleData::connect(const std::string& signal, Slot* slot)
{
	auto pair = this->m_slots.find(signal);
	if (pair != this->m_slots.end())
	{
		pair->second.push_back(std::shared_ptr<Slot>(slot));
	}
	return slot;
}

void ZDSJ::DrawAbleData::applyForce(const DrawAbleData* _other) const
{
	this->m_movement->applyForce(_other->m_movement.get());
}

void ZDSJ::DrawAbleData::update(float _magnification)
{
	this->m_movement->updatePosition(this->m_position, _magnification);
	memcpy(&this->m_renderSize, &this->m_size, sizeof(DirectX::XMFLOAT3));
	memcpy(&this->m_renderPosition, &this->m_position, sizeof(DirectX::XMFLOAT3));
	memcpy(&this->m_renderRotation, &this->m_rotation, sizeof(DirectX::XMFLOAT3));
}

ZDSJ::DrawAbleData* ZDSJ::DrawAbleData::addVelocity(const DirectX::XMFLOAT3& _velocity)
{
	this->m_movement->addVelocity(_velocity);
	return this;
}

void ZDSJ::DrawAbleData::registerSignal()
{
	this->m_slots.insert(std::make_pair("position", std::vector<std::shared_ptr<Slot>>()));
}
