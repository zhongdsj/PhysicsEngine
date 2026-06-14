// #pragma kernel csMain

// ---------------------------
// Particle data structure (full physics & material properties)
// ---------------------------
struct Particle
{
	// Physics
	float3 position;    // position
	float3 velocity;    // velocity
	float3 force;       // force

	// Material Properties
	float mass;         // mass
	float charge;       // charge (electromagnetic force)
	float energy;       // energy
	float temperature;  // temperature
	float density;      // density
	float stability;    // stability (aggregation)
	float type;         // element type
	float id;           // unique ID from CPU
};

// Double buffer system
// Read-only: previous frame state
StructuredBuffer<Particle> in_particle_buffer;

// Write-only: current frame output
RWStructuredBuffer<Particle> out_particle_buffer;

// ---------------------------
// Basic physics parameters (updated by CPU each frame)
// ---------------------------
cbuffer PhysicsParams : register(b0)
{
	float   delta_time;          // time step
	int     particle_count;      // total particle count
	float   gravity_scale;       // gravity strength
	float   electromagnetic_scale;       // electromagnetic force strength
	float   strong_force_range;   // strong force radius
	float   weak_force_range;     // weak force radius
	float   damping;            // velocity damping
};

// Chaos coupling parameters
// Controls how properties influence each other
cbuffer ChaosParams : register(b1)
{
	float   energy_to_mass;         // energy to mass ratio (e.g. 0.1)
	float   temp_to_electromagnetic;        // temperature boost to electromagnetic force (e.g. 0.5)
	float   heat_repulsion_scale;   // heat repulsion ratio (e.g. 0.8)
	float   strong_force_base;      // base strong force strength (e.g. 20.0)
	float   energy_to_strong;       // energy boost to strong force (e.g. 5.0)
	float   stability_decay;        // natural stability decay (e.g. 0.005)
	float   temp_over_heat_penalty; // stability penalty from overheating (e.g. 0.02)
	float   energy_over_heat_penalty;// stability penalty from over energy (e.g. 0.01)
	float   stability_from_strong; // stability gain from strong force (e.g. 0.05)
	float   decay_max_stability; // max stability for decay (e.g. 0.3f)
	float   strong_energy; // energy change from strong force (e.g. 0.1f)
	float   strong_temperature; // temperature change from strong force (e.g. 0.05f)
	float   weak_energy; // energy change from weak force (e.g. 0.2f)
	float   weak_temperature; // temperature change from weak force (e.g. 0.1f)
	float   temperature_max_stability; // max temperature for stability (e.g. 1.0f)
	float   energy_max_stability; // max energy for stability (e.g. 2.0f)
};


// World state parameters
// Natural decay & material formation rules
cbuffer WorldParams: register(b2)
{
	float step_temperature; // natural temperature decrease (e.g. 0.01f)
	float min_temperature; // minimum temperature (0.0f)
	float max_temperature; // maximum temperature (2.0f)
	float step_energy; // natural energy decrease (e.g. 0.02f)
	float min_energy; // minimum energy (0.0f)
	float max_energy; // maximum energy (5.0f)
	float step_stability; // density increase after stability threshold (e.g. 0.005f)
	float min_stability; // stability threshold (e.g. 0.8f)
};

// Calculate all forces between two particles
void computeForces(inout Particle self, in Particle other)
{
	float3 dir = other.position - self.position;
	float dist = length(dir);
	if (dist < 0.001f) return;
	float3 norm_dir = normalize(dir);

	// Gravity: energy contributes to effective mass
	float self_eff_mass = self.mass + self.energy * energy_to_mass;
	float other_eff_mass = other.mass + other.energy * energy_to_mass;
	float gravity = (self_eff_mass * other_eff_mass) / (dist * dist + 1.0f);
	self.force += norm_dir * gravity * gravity_scale;

	// Electromagnetic force: enhanced by temperature
	float temp_factor = 1.0f + self.temperature * temp_to_electromagnetic;
	float electromagnetic = -(self.charge * other.charge * temp_factor) / (dist * dist + 1.0f);
	self.force += norm_dir * electromagnetic * electromagnetic_scale;

	// Heat repulsion
	float heat_force = (self.temperature + other.temperature) / (dist + 1.0f);
	self.force += norm_dir * heat_force * heat_repulsion_scale;

	// Strong nuclear interaction (binding)
	if (dist < strong_force_range)
	{
		float strong_strength = strong_force_base + self.energy * energy_to_strong;
		self.force += norm_dir * strong_strength;

		self.stability = min(1.0f, self.stability + stability_from_strong);  // 上升
		self.energy += strong_energy;
		self.temperature += strong_temperature;
	}

	// Weak interaction (decay)
	if (dist < weak_force_range && self.stability < decay_max_stability)
	{
		self.energy -= weak_energy;
		self.temperature += weak_temperature;
	}

	// --------------------------
	// Natural stability decay
	// --------------------------
	self.stability = max(0.0f, self.stability - stability_decay);

	// High temperature reduces stability
	if (self.temperature > temperature_max_stability)
	{
		self.stability = max(0.0f, self.stability - temp_over_heat_penalty);
	}

	// High energy reduces stability
	if (self.energy > energy_max_stability)
	{
		self.stability = max(0.0f, self.stability - energy_over_heat_penalty);
	}
}

// Main compute kernel entry point
[numthreads(64, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
	int idx = id.x;
	if (idx >= particle_count) return;

	Particle p = in_particle_buffer[idx];

	p.force = float3(0, 0, 0);

	for (int i = 0; i < particle_count; i++)
	{
		if (i == idx) continue;
		Particle other = in_particle_buffer[i];
		computeForces(p, other);
	}

	// Semi-Implicit Euler Integration
	p.velocity += p.force / max(p.mass, 0.001f) * delta_time;
	p.velocity *= damping;
	p.position += p.velocity * delta_time;

	// Natural property evolution
	p.temperature = clamp(p.temperature - step_temperature, min_temperature, max_temperature);
	p.energy = clamp(p.energy - step_energy, min_energy, max_energy);

	// High stability increases density (material formation)
	if (p.stability > min_stability)
	{
		p.density += step_stability;
	}

	// Write final particle state to output buffer
	out_particle_buffer[idx] = p;
}