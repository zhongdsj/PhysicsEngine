#pragma once

#include <d3d11.h>
#include <vector>
#include <wrl.h>

using namespace Microsoft::WRL;

namespace ZDSJ
{
	struct Particle;

	class ParticlePhysicsManager
	{
	public:
		ParticlePhysicsManager(ID3D11Device* device,
		                       ID3D11DeviceContext* context) :m_device(device), m_context(context){}

		bool initialize(
			const std::vector<Particle>& _initialParticles);

		bool initialized() const { return m_initialized; };

		void update(float _deltaTime);

		void render() const;

		// 获取当前粒子数据（用于渲染）
		const std::vector<Particle>& getParticles() const { return m_cpuParticles; }

		void release();

	private:
		bool loadComputeShader(const wchar_t* path);
		bool loadVertexShader(const wchar_t* path);
		bool loadPixelShader(const wchar_t* path);
		bool loadGeometryShader(const wchar_t* path);
		void createStructuredBuffers();
		void updateConstantBuffers(float _deltaTime);
		void createConstantBuffers();

	private:
		ComPtr<ID3D11Device> m_device;
		ComPtr<ID3D11DeviceContext> m_context;
		ComPtr<ID3D11ComputeShader> m_cs;
		ComPtr<ID3D11VertexShader> m_vertexShader;
		ComPtr<ID3D11PixelShader> m_pixelShader;
		ComPtr<ID3D11GeometryShader> m_geoShader;

		// 双缓冲
		ComPtr<ID3D11Buffer>                    m_particleBuffer[2];
		ComPtr<ID3D11ShaderResourceView>        m_srv[2];
		ComPtr<ID3D11UnorderedAccessView>        m_uav[2];
		int m_readBuffer = 0;     // 读
		int m_writeBuffer = 1;    // 写

		// 常量缓冲区
		ComPtr<ID3D11Buffer> m_cbPhysics;
		ComPtr<ID3D11Buffer> m_cbChaos;
		ComPtr<ID3D11Buffer> m_cbWorld;

		std::vector<Particle> m_cpuParticles;
		int m_particleCount = 0;
		bool m_initialized = false;
		bool m_isLoaded = false;
		const int THREAD_GROUP_SIZE = 64;
	};
}
