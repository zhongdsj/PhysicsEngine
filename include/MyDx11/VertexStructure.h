#pragma once
# include <DirectXMath.h>

namespace ZDSJ {

	struct TransformConstantBuffer {
		DirectX::XMMATRIX translation;
		DirectX::XMMATRIX scal;
		DirectX::XMMATRIX rotation;
	};

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