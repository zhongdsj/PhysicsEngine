# pragma once

namespace ZDSJ
{
	struct Point {
		float x;
		float y;
		Point(float _x, float _y) :x(_x), y(_y) {}
		Point() :x(0.0f), y(0.0f) {}
	};

	struct Position
	{
		float x;
		float y;
		float z;

		Position(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}
		Position() :x(0.0f), y(0.0f), z(0.0f) {}
	};
}
