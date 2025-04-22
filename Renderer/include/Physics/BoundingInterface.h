# pragma once

namespace DirectX
{
	class BoundingSphere;
	class BoundingBox;
	class BoundingOrientedBox;
	class BoundingFrustum;
}

namespace ZDSJ
{
	class Ray;
	class BoundingInterface
	{
	public:
		BoundingInterface() = default;
		/**
		 * @brief 球形碰撞
		 * @param _other 
		 * @return 
		 */
		virtual bool intersects(const DirectX::BoundingSphere& _other) const = 0;
		/**
		 * @brief 盒形碰撞
		 * @param _other
		 * @return
		 */
		virtual bool intersects(const DirectX::BoundingBox& _other) const = 0;
		/**
		 * @brief 边界框碰撞
		 * @param _other
		 * @return
		 */
		virtual bool intersects(const DirectX::BoundingOrientedBox& _other) const = 0;
		/**
		 * @brief 视锥碰撞
		 * @param _other
		 * @return
		 */
		virtual bool intersects(const DirectX::BoundingFrustum& _other) const = 0;
		/**
		 * @brief 光线碰撞检测
		 * @param _ray 
		 * @return 
		 */
		virtual bool intersects(const Ray& _ray) const = 0;

		virtual void movementChanged(const DirectX::XMFLOAT3& _size, const DirectX::XMFLOAT3& _position, const DirectX::XMFLOAT3& _rotation) = 0;
		virtual ~BoundingInterface() = default;
	};
}
