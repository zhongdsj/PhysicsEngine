# pragma once
# include <Physics/PhysicsInterface.h>
# include <Physics/BoundingInterface.h>
# include <DirectXCollision.h>
# include <memory>

namespace ZDSJ
{
	class Ray;
	
	enum class BoundingType
	{
		sphere = 0,
		box,
		custom
	};

	struct BoundingObject
	{
		BoundingType bounding_type;
		union
		{
			DirectX::BoundingSphere sphere;
			DirectX::BoundingBox box;
			BoundingInterface* custom;
		};
		BoundingObject(BoundingType _type, const DirectX::BoundingSphere& _sphere): bounding_type(_type), sphere(_sphere){}
		BoundingObject(BoundingType _type, const DirectX::BoundingBox& _box): bounding_type(_type), box(_box){}
		BoundingObject(BoundingType _type, BoundingInterface* _custom): bounding_type(_type), custom(_custom){}
	};

	class CollisionInterface : public PhysicsInterface
	{
	public:
		CollisionInterface(MovementInterface* _movement) : PhysicsInterface(_movement) {}
		virtual bool intersects(CollisionInterface* _other) const = 0;
		virtual bool intersects(const Ray& _ray) const = 0;
		const std::vector<std::shared_ptr<BoundingObject>>& getBounding() const { return this->m_bounding; }
		virtual ~CollisionInterface() override = default;
	protected:
		virtual void movementChanged() = 0;
		std::vector<std::shared_ptr<BoundingObject>> m_bounding;
	};
}
