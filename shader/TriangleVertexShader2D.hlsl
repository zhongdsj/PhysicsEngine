struct VertexShaderOut {
	float4 color : Color;
	float4 pos : SV_Position;
};

VertexShaderOut main_vertex_shader(float2 pos : Position, float4 color: Color)
{
	VertexShaderOut vso;
	vso.pos = float4(pos.x, pos.y, 0.0f, 1.0f);
	vso.color = color;
	return vso;
}