# pragma once

namespace ZDSJ
{
	struct Vector;

	class MovementInterface
	{

	public:
		virtual Vector calculate(MovementInterface* _other) = 0;
		virtual float getQuality() = 0;
		virtual ~MovementInterface() = default;

	};

}
