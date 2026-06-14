struct VertexShaderOut {
	float4 color : Color;
	float4 pos : SV_Position;
};

cbuffer ConstantBuffer {
	matrix transform;
};

VertexShaderOut main_vertex_shader(float2 _pos : Position, float4 _color: Color)
{
	VertexShaderOut vso;
	vso.pos = mul(float4(_pos.x, _pos.y, 0.0f, 1.0f), transform);
	vso.color = _color;
	return vso;
}