# include <Manager/BaseManager.h>
# include <MyDx11/DrawAbleData.h>
# include <MyDx11/DrawAble/Arc2DDrawAble.h>
# include <MyDx11/Ray.h>
# include <MyDx11/Camera.h>
# include <Context.h>
# include <DirectXMath.h>
# include <Physics/CollisionInterface.h>
# include <Physics/MovementInterface.h>

ZDSJ::BaseManager::BaseManager(ID3D11Device* _device, ID3D11DeviceContext* _context)
{
	this->m_render.insert_or_assign("Arc2D", std::make_shared<Arc2DDrawAble>(_device, _context, 20, 360));
}

void ZDSJ::BaseManager::add(DrawAbleData* _data)
{
	this->m_data.push_back(std::shared_ptr<DrawAbleData>(_data));
}

void ZDSJ::BaseManager::render(ID3D11DeviceContext* _context)
{
	using namespace DirectX;
	// 获取鼠标位置，创建射线
	auto position = Context_Instance->getMouseWorld();
	XMVECTOR views_pace_start_point = XMVectorSet(Camera_Instance->cameraPos().x, Camera_Instance->cameraPos().y, Camera_Instance->cameraPos().z, 0.0f);
	XMVECTOR views_pace_end_point = XMVectorSet(position.x, position.y, position.z, 0.0f);
	XMVECTOR direction = XMVector3Normalize(views_pace_end_point - views_pace_start_point);
	Ray ray(views_pace_start_point, direction, Camera_Instance->cameraFarPlane());
	for(int i = 0; i < this->m_data.size(); ++i)
	{
		auto& wait_render = this->m_data.at(i);
		//鼠标发出射线碰撞
		if(Keyboard_Instance->isKeyDown(SpecialKey::key_ctrl) && wait_render->movement()->getCollision()->intersects(ray))
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
}

void ZDSJ::BaseManager::physicsCalculate(float speed)
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
		wait_render->update(speed);
	}
}

bool ZDSJ::BaseManager::skipCalculate(const DrawAbleData* _first, const DrawAbleData* _second)
{
	return _first == _second;
}
