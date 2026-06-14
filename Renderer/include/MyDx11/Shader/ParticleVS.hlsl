#pragma vertex main
struct Particle
{
	float3 position;
	float3 velocity;
	float3 force;
	float mass;
	float charge;
	float energy;
	float temperature;
	float density;
	float stability;
	float type;
	float id;
};

StructuredBuffer<Particle> particle_buffer : register(t0);

struct VSOutput
{
	float3 world_pos : TEXCOORD0;
	float4 color : COLOR;
	float size : TEXCOORD0;
};


VSOutput main(uint id : SV_VertexID)
{
	VSOutput output;
	Particle p = particle_buffer[id];
	output.world_pos = p.position;
	float r = saturate(max(p.temperature * 0.5f, 0.1f));
	float g = saturate(max(p.stability, 0.1f));
	float b = saturate(max(p.energy * 0.2f, 0.1f));
	output.color = float4(r, g, b, 1.0f);
	// output.color = float4(1.0f, 1.0f, 1.0f, 1.0f);
	output.size = 10.0f;

	return output;
}