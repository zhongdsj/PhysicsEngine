# pragma once

namespace ZDSJ
{
	class ComponentInterface
	{
	public:
		virtual void tick(float _use_time) = 0;
		virtual void logicTick(float speed) { }
		virtual ~ComponentInterface() = default;
	};
}
