# include <Manager/BaseManager.h>
# include <MyDx11/DrawAbleData.h>
# include <MyDx11/DrawAble/Arc2DDrawAble.h>
# include <MyDx11/Ray.h>
# include <MyDx11/Camera.h>
# include <Context.h>
# include <DirectXMath.h>
# include <Physics/CollisionInterface.h>
# include <Physics/MovementInterface.h>
# include <Physics/ParticlePhysicsManager.h>
# include <Physics/ParticalePhysicsData.h>

ZDSJ::BaseManager::BaseManager(ID3D11Device* _device, ID3D11DeviceContext* _context)
{
	this->m_render.insert_or_assign("Arc2D", std::make_shared<Arc2DDrawAble>(_device, _context, 20, 360));
	this->m_particlePhysicsManager = new ParticlePhysicsManager(_device, _context);
}

void ZDSJ::BaseManager::add(DrawAbleData* _data)
{
	this->m_data.push_back(std::shared_ptr<DrawAbleData>(_data));
}

void ZDSJ::BaseManager::render(ID3D11DeviceContext* _context)
{
	using namespace DirectX;
	// 获取鼠标位置，创建射线
	// TODO 这部分后续要根据是否在gpu中处理来做
	if((this->m_particlePhysicsManager != nullptr && !this->m_particlePhysicsManager->initialized()) || this->m_particlePhysicsManager == nullptr)
	{
		auto position = Context_Instance->getMouseWorld();
		XMVECTOR views_pace_start_point = XMVectorSet(Camera_Instance->cameraPos().x, Camera_Instance->cameraPos().y, Camera_Instance->cameraPos().z, 0.0f);
		XMVECTOR views_pace_end_point = XMVectorSet(position.x, position.y, position.z, 0.0f);
		XMVECTOR direction = XMVector3Normalize(views_pace_end_point - views_pace_start_point);
		Ray ray(views_pace_start_point, direction, Camera_Instance->cameraFarPlane());
		for (int i = 0; i < this->m_data.size(); ++i)
		{
			auto& wait_render = this->m_data.at(i);
			//鼠标发出射线碰撞
			if (Keyboard_Instance->isKeyDown(SpecialKey::key_ctrl) && wait_render->movement()->getCollision()->intersects(ray))
			{
				// TODO 渲染到屏幕上
				Log_Debug("{}", wait_render->printProperties());
			}

			// 渲染
			auto pair = this->m_render.find(wait_render->renderType());
			if (pair != this->m_render.end())
			{
				pair->second->draw(_context, wait_render.get());
			}
		}
	} else
	{
		this->m_particlePhysicsManager->render();
	}

	// 按键判断
	if (Keyboard_Instance->isKeyPressed(SpecialKey::key_t))
	{
		// 开启gpu模拟
		if(this->m_particlePhysicsManager == nullptr)
		{
			Log_Error("开启gpu模拟失败, 空指针");
		} else
		{
			std::vector<Particle> data;
			for(int i = 0; i < this->m_data.size(); ++i)
			{
				Particle particle = {};
				particle.mass = 0.5f;
				particle.charge = 0.5f;
				particle.energy = 0.5f;
				particle.temperature = 0.5f;
				particle.density = 0.0f;
				particle.stability = 0.0f;
				particle.type = 1;
				particle.id = i;
				auto draw_able_data = this->m_data.at(i);
				particle.position = draw_able_data.get()->position();
				particle.velocity = { 0.0f, 0.0f, 0.0f };
				particle.force = { 0.0f, 0.0f, 0.0f };
				data.emplace_back(particle);
			}
			this->m_particlePhysicsManager->initialize(data);
			if(this->m_particlePhysicsManager->initialized())
			{
				Log_Info("gpu模拟初始化成功");
			} else
			{
				Log_Error("gpu模拟初始化失败");
				this->m_particlePhysicsManager->release();
			}
		}
	}

	if (Keyboard_Instance->isKeyPressed(SpecialKey::key_r))
	{
		if (this->m_particlePhysicsManager == nullptr)
		{
			Log_Error("关闭gpu模拟失败, 空指针");
		}
		else if(this->m_particlePhysicsManager->initialized())
		{
			this->m_particlePhysicsManager->release();
			Log_Debug("关闭gpu模拟成功");
		} else
		{
			Log_Debug("关闭gpu模拟失败, 未初始化");
		}
	}
}

void ZDSJ::BaseManager::physicsCalculate(float _deltaTime)
{
	if((this->m_particlePhysicsManager != nullptr && !this->m_particlePhysicsManager->initialized()) || this->m_particlePhysicsManager == nullptr)
	{
		for (int i = 0; i < this->m_data.size(); ++i)
		{
			auto& wait_render = this->m_data.at(i);
			for (int j = i + 1; j < this->m_data.size(); ++j)
			{
				auto& wait_calculate = this->m_data.at(j);
				if (this->skipCalculate(wait_render.get(), wait_calculate.get()))
				{
					continue;
				}
				wait_render->applyForce(wait_calculate.get());
			}
			wait_render->update(_deltaTime);
		}
	} else
	{
		this->m_particlePhysicsManager->update(_deltaTime);
	}

}

bool ZDSJ::BaseManager::skipCalculate(const DrawAbleData* _first, const DrawAbleData* _second) const
{
	return _first == _second;
}
