# pragma once

namespace ZDSJ
{
	class ComponentInterface
	{
	public:
		virtual void tick(float _use_time) = 0;
		virtual ~ComponentInterface() = default;
	};
}
