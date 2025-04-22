# pragma once
# include <DirectXMath.h>

namespace ZDSJ
{
	class Ray
	{
	public:
		Ray(const DirectX::XMFLOAT3& _origin, const DirectX::XMFLOAT3& _direction, const float& _dist): m_origin(DirectX::XMLoadFloat3(&_origin)), m_direction(DirectX::XMLoadFloat3(&_direction)), m_dist(_dist)
		{
		};

		Ray(const DirectX::XMVECTOR& _origin, const DirectX::XMVECTOR& _direction, const float& _dist) : m_origin(_origin), m_direction(_direction), m_dist(_dist)
		{
		};

		DirectX::FXMVECTOR origin() const
		{
			return m_origin;
		}

		DirectX::FXMVECTOR direction() const
		{
			return m_direction;
		}

		float dist() const
		{
			return this->m_dist;
		}

		~Ray() = default;
	private:
		DirectX::XMVECTOR m_origin;
		DirectX::XMVECTOR m_direction;
		float m_dist;
	};
}
