# include <fstream>
# include <Physics/ParticlePhysicsManager.h>
# include <Logger.h>
# include <Physics/ParticalePhysicsData.h>

bool ZDSJ::ParticlePhysicsManager::initialize(const std::vector<Particle>& _initialParticles)
{
	m_cpuParticles = _initialParticles;
	m_particleCount = static_cast<int>(_initialParticles.size());
	m_isLoaded = true;
	m_isLoaded &= this->loadComputeShader(L"ParticlePhysics.cso");
	m_isLoaded &= this->loadVertexShader(L"ParticleVS.cso");
	m_isLoaded &= this->loadPixelShader(L"ParticlePS.cso");
	m_isLoaded &= this->loadGeometryShader(L"ParticleGS.cso");
	if (!m_isLoaded)
		return false;

	createStructuredBuffers();
	createConstantBuffers();
	this->m_initialized = true;
	return true;
}

void ZDSJ::ParticlePhysicsManager::update(float _deltaTime)
{
	this->updateConstantBuffers(_deltaTime);

	// 绑定 Compute Shader
	m_context->CSSetShader(m_cs.Get(), nullptr, 0);

	// 绑定 双缓冲
	m_context->CSSetShaderResources(0, 1, m_srv[m_readBuffer].GetAddressOf());
	m_context->CSSetUnorderedAccessViews(0, 1, m_uav[m_writeBuffer].GetAddressOf(), nullptr);

	// 绑定 3 个常量缓冲区
	m_context->CSSetConstantBuffers(0, 1, m_cbPhysics.GetAddressOf());
	m_context->CSSetConstantBuffers(1, 1, m_cbChaos.GetAddressOf());
	m_context->CSSetConstantBuffers(2, 1, m_cbWorld.GetAddressOf());

	// 执行 Dispatch
	int group_count = (m_particleCount + THREAD_GROUP_SIZE - 1) / THREAD_GROUP_SIZE;
	m_context->Dispatch(group_count, 1, 1);

	// 帧结束：交换索引
	std::swap(m_readBuffer, m_writeBuffer);
	// 清理绑定
	ID3D11ShaderResourceView* null_srv = nullptr;
	ID3D11UnorderedAccessView* null_uav = nullptr;
	m_context->CSSetShaderResources(0, 1, &null_srv);
	m_context->CSSetUnorderedAccessViews(0, 1, &null_uav, nullptr);
}

void ZDSJ::ParticlePhysicsManager::render() const
{
	m_context->IASetInputLayout(nullptr);
	// 1. 设置拓扑为点
	m_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

	// 2. 绑定渲染着色器
	m_context->VSSetShader(m_vertexShader.Get(), nullptr, 0);
	m_context->PSSetShader(m_pixelShader.Get(), nullptr, 0);
	m_context->GSSetShader(m_geoShader.Get(), nullptr, 0);

	// 3. 绑定粒子数据（✅ 正确写法，不报错）
	ID3D11ShaderResourceView* srv = m_srv[m_readBuffer].Get();
	m_context->VSSetShaderResources(0, 1, &srv);

	// 4. 绘制
	m_context->Draw(m_particleCount, 0);

	// 5. 渲染完解绑（防止污染下一帧）
	ID3D11ShaderResourceView* null_srv = nullptr;
	m_context->VSSetShaderResources(0, 1, &null_srv);
	// 渲染完毕解绑 GS
	m_context->GSSetShader(nullptr, nullptr, 0);
}

void ZDSJ::ParticlePhysicsManager::updateConstantBuffers(float _deltaTime)
{
	// 你可以在这里动态调整物理参数
	PhysicsParams phys{};
	phys.delta_time = _deltaTime;
	phys.particle_count = m_particleCount;
	phys.gravity_scale = 0.1f;
	phys.electromagnetic_scale = 0.2f;
	phys.strong_force_range = 1.0f;
	phys.weak_force_range = 2.0f;
	phys.damping = 0.99f;

	ChaosParams chaos{};
	chaos.energy_to_mass = 0.1f;
	chaos.temp_to_electromagnetic = 0.5f;
	chaos.heat_repulsion_scale = 0.8f;
	chaos.strong_force_base = 20.0f;
	chaos.energy_to_strong = 5.0f;
	chaos.stability_decay = 0.005f;
	chaos.temp_over_heat_penalty = 0.02f;
	chaos.energy_over_heat_penalty = 0.01f;
	chaos.stability_from_strong = 0.05f;
	chaos.decay_max_stability = 0.3f;
	chaos.strong_energy = 0.1f;
	chaos.strong_temperature = 0.05f;
	chaos.weak_energy = 0.2f;
	chaos.weak_temperature = 0.1f;
	chaos.temperature_max_stability = 1.0f;
	chaos.energy_max_stability = 2.0f;

	WorldParams world{};
	world.step_temperature = 0.01f;
	world.min_temperature = 0.0f;
	world.max_temperature = 2.0f;
	world.step_energy = 0.02f;
	world.min_energy = 0.0f;
	world.max_energy = 5.0f;
	world.step_stability = 0.005f;
	world.min_stability = 0.8f;


	m_context->UpdateSubresource(m_cbPhysics.Get(), 0, nullptr, &phys, 0, 0);
	m_context->UpdateSubresource(m_cbChaos.Get(), 0, nullptr, &chaos, 0, 0);
	m_context->UpdateSubresource(m_cbWorld.Get(), 0, nullptr, &world, 0, 0);
}

void ZDSJ::ParticlePhysicsManager::release()
{
	for (auto& particle_buffer : m_particleBuffer)
	{
		particle_buffer.Reset();
	}
	for (auto& srv : m_srv)
	{
		srv.Reset();
	}
	for (auto& uav : m_uav)
	{
		uav.Reset();
	}
	m_cbPhysics.Reset();
	m_cbChaos.Reset();
	m_cbWorld.Reset();
	m_particleCount = 0;
	m_initialized = false;

	// 确保 GPU 立刻释放
	/*m_context->ClearState();
	m_context->Flush();*/
}

bool ZDSJ::ParticlePhysicsManager::loadComputeShader(const wchar_t* path)
{
	std::ifstream file_stream(path, std::ios::binary | std::ios::ate);
	if (!file_stream)
	{
		Log_Error("读取基础物理cso文件失败, 无法打开文件: {}", Logger::wchar_to_char(path));
		return false;
	}

	// 2. 读取大小
	size_t file_size = (size_t)file_stream.tellg();
	file_stream.seekg(0);

	// 3. 读取数据
	std::vector<char> data(file_size);
	file_stream.read(data.data(), file_size);

	HRESULT hr = m_device->CreateComputeShader(
		data.data(),
		data.size(),
		nullptr,
		&m_cs);

	if (FAILED(hr))
	{
		Log_Error("基础物理cso CreateComputeShader 失败");
		m_isLoaded = false;
		return false;
	}

	Log_Info("基础物理cso CreateComputeShader 成功");
	return SUCCEEDED(hr);
}

bool ZDSJ::ParticlePhysicsManager::loadVertexShader(const wchar_t* path)
{
	std::ifstream file_stream(path, std::ios::binary | std::ios::ate);
	if (!file_stream)
	{
		Log_Error("读取基础物理cso文件失败, 无法打开文件: {}", Logger::wchar_to_char(path));
		return false;
	}

	// 2. 读取大小
	size_t file_size = (size_t)file_stream.tellg();
	file_stream.seekg(0);

	// 3. 读取数据
	std::vector<char> data(file_size);
	file_stream.read(data.data(), file_size);

	HRESULT hr = m_device->CreateVertexShader(
		data.data(),
		data.size(),
		nullptr,
		&m_vertexShader);

	if (FAILED(hr))
	{
		Log_Error("基础物理cso CreateVertexShader 失败");
		return false;
	}

	Log_Info("基础物理cso CreateVertexShader 成功");
	return SUCCEEDED(hr);
}

bool ZDSJ::ParticlePhysicsManager::loadPixelShader(const wchar_t* path)
{
	std::ifstream file_stream(path, std::ios::binary | std::ios::ate);
	if (!file_stream)
	{
		Log_Error("读取基础物理cso文件失败, 无法打开文件: {}", Logger::wchar_to_char(path));
		return false;
	}

	// 2. 读取大小
	size_t file_size = (size_t)file_stream.tellg();
	file_stream.seekg(0);

	// 3. 读取数据
	std::vector<char> data(file_size);
	file_stream.read(data.data(), file_size);

	HRESULT hr = m_device->CreatePixelShader(
		data.data(),
		data.size(),
		nullptr,
		&m_pixelShader);

	if (FAILED(hr))
	{
		Log_Error("基础物理cso CreatePixelShader 失败");
		return false;
	}

	Log_Info("基础物理cso CreatePixelShader 成功");
	return SUCCEEDED(hr);
}

bool ZDSJ::ParticlePhysicsManager::loadGeometryShader(const wchar_t* path)
{
	std::ifstream file_stream(path, std::ios::binary | std::ios::ate);
	if (!file_stream)
	{
		Log_Error("读取基础物理cso文件失败, 无法打开文件: {}", Logger::wchar_to_char(path));
		return false;
	}

	// 2. 读取大小
	size_t file_size = (size_t)file_stream.tellg();
	file_stream.seekg(0);

	// 3. 读取数据
	std::vector<char> data(file_size);
	file_stream.read(data.data(), file_size);

	HRESULT hr = m_device->CreateGeometryShader(
		data.data(),
		data.size(),
		nullptr,
		&m_geoShader);

	if (FAILED(hr))
	{
		Log_Error("基础物理cso CreateGeometryShader 失败");
		return false;
	}

	Log_Info("基础物理cso CreateGeometryShader 成功");
	return SUCCEEDED(hr);
}

void ZDSJ::ParticlePhysicsManager::createStructuredBuffers()
{
	// 创建结构化缓冲区（A读，B写）
	D3D11_BUFFER_DESC desc{};
	desc.ByteWidth = m_particleCount * sizeof(Particle);
	desc.StructureByteStride = sizeof(Particle);
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

	for (int i = 0; i < 2; i++)
	{
		m_device->CreateBuffer(&desc, nullptr, &m_particleBuffer[i]);

		D3D11_SHADER_RESOURCE_VIEW_DESC srv_desc{};
		srv_desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
		srv_desc.Buffer.NumElements = m_particleCount;
		m_device->CreateShaderResourceView(m_particleBuffer[i].Get(), &srv_desc, &m_srv[i]);

		D3D11_UNORDERED_ACCESS_VIEW_DESC uav_desc{};
		uav_desc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
		uav_desc.Buffer.NumElements = m_particleCount;
		m_device->CreateUnorderedAccessView(m_particleBuffer[i].Get(), &uav_desc, &m_uav[i]);
	}

	// 上传初始数据
	m_context->UpdateSubresource(m_particleBuffer[0].Get(), 0, nullptr, m_cpuParticles.data(), 0, 0);
}

void ZDSJ::ParticlePhysicsManager::createConstantBuffers()
{
	// 创建并绑定 CB
	D3D11_BUFFER_DESC physics_params_desc{};
	physics_params_desc.ByteWidth = align16(sizeof(PhysicsParams));
	physics_params_desc.Usage = D3D11_USAGE_DEFAULT;
	physics_params_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	physics_params_desc.CPUAccessFlags = 0;

	D3D11_BUFFER_DESC chaos_params_desc{};
	chaos_params_desc.ByteWidth = align16(sizeof(ChaosParams));
	chaos_params_desc.Usage = D3D11_USAGE_DEFAULT;
	chaos_params_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	chaos_params_desc.CPUAccessFlags = 0;

	D3D11_BUFFER_DESC world_params_desc{};
	world_params_desc.ByteWidth = align16(sizeof(WorldParams));
	world_params_desc.Usage = D3D11_USAGE_DEFAULT;
	world_params_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	world_params_desc.CPUAccessFlags = 0;

	m_device->CreateBuffer(&physics_params_desc, nullptr, &m_cbPhysics);
	m_device->CreateBuffer(&chaos_params_desc, nullptr, &m_cbChaos);
	m_device->CreateBuffer(&world_params_desc, nullptr, &m_cbWorld);
}
