#pragma once

#include <DirectXMath.h>
namespace ZDSJ
{
	inline UINT align16(UINT size)
	{
		return (size + 15) & ~15;
	}

	// 粒子结构体
	struct Particle
	{
		DirectX::XMFLOAT3 position;        // 位置
		DirectX::XMFLOAT3 velocity;        // 速度
		DirectX::XMFLOAT3 force;           // 受力

		float mass;               // 质量          示例：1.0f
		float charge;             // 电荷          示例：-1.0f / +1.0f
		float energy;             // 能量          示例：0.0f ~ 5.0f
		float temperature;        // 温度          示例：0.0f ~ 2.0f
		float density;            // 密度          示例：0.0f 逐渐累积
		float stability;          // 稳定性        示例：0.0f ~ 1.0f
		float type;               // 元素类型      示例：0,1,2,3 区分粒子类型
		float id;                 // 唯一ID        示例：索引值
	};

	// 物理基础参数（对应 HLSL: register(b0)）
	struct alignas(16) PhysicsParams
	{
		float delta_time;             // 时间步长        示例：1.0f / 60.0f
		int   particle_count;         // 粒子总数        示例：1024
		float gravity_scale;          // 引力强度        示例：0.1f
		float electromagnetic_scale;  // 电磁力强度      示例：0.2f
		float strong_force_range;     // 强力作用半径    示例：1.0f
		float weak_force_range;       // 弱力作用半径    示例：2.0f
		float damping;                // 速度阻尼        示例：0.99f
	};

	// 混沌耦合参数（对应 HLSL: register(b1)）
	struct alignas(16) ChaosParams
	{
		float energy_to_mass;            // 能量转质量比例          示例：0.1f
		float temp_to_electromagnetic;   // 温度增强电磁力比例      示例：0.5f
		float heat_repulsion_scale;      // 热斥力强度比例          示例：0.8f
		float strong_force_base;         // 强力基础强度            示例：20.0f
		float energy_to_strong;          // 能量增强强力比例        示例：5.0f
		float stability_decay;           // 稳定性自然衰减速率      示例：0.005f
		float temp_over_heat_penalty;    // 过热稳定性惩罚值        示例：0.02f
		float energy_over_heat_penalty;  // 过能量稳定性惩罚值      示例：0.01f
		float stability_from_strong;     // 强力提升稳定性值        示例：0.05f
		float decay_max_stability;       // 衰变最大稳定性阈值      示例：0.3f
		float strong_energy;             // 强力作用能量变化        示例：0.1f
		float strong_temperature;        // 强力作用温度变化        示例：0.05f
		float weak_energy;               // 弱力作用能量变化        示例：0.2f
		float weak_temperature;          // 弱力作用温度变化        示例：0.1f
		float temperature_max_stability; // 稳定温度上限            示例：1.0f
		float energy_max_stability;      // 稳定能量上限            示例：2.0f
	};

	// 世界规则参数（对应 HLSL: register(b2)）
	struct alignas(16) WorldParams
	{
		float step_temperature;     // 温度自然下降速率    示例：0.01f
		float min_temperature;      // 温度下限            示例：0.0f
		float max_temperature;      // 温度上限            示例：2.0f
		float step_energy;          // 能量自然衰减速率    示例：0.02f
		float min_energy;           // 能量下限            示例：0.0f
		float max_energy;           // 能量上限            示例：5.0f
		float step_stability;       // 稳定后密度增长速度  示例：0.005f
		float min_stability;        // 材料固化稳定阈值    示例：0.8f
	};

	// 粒子渲染参数（对应 HLSL: register(b0)，仅 VS/PS 使用）
	struct alignas(16) RenderParams
	{
		DirectX::XMFLOAT4X4 view_projection;
		float point_size;
		float padding[3];
	};
}
