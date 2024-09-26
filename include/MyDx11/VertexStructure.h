#pragma once


namespace ZDSJ {

	struct Vertex2D {
		struct {
			float x; float y;
		} pos;
		struct {
			unsigned char r;
			unsigned char g;
			unsigned char b;
			unsigned char a;
		} color;
	};
}