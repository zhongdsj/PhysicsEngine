# pragma once

namespace ZDSJ
{
	struct Point {
		float x;
		float y;
		Point(float _x, float _y) :x(_x), y(_y) {};
		Point() :x(0.0f), y(0.0f) {};
	};
}
