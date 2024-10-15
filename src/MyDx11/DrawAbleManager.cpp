# include <MyDx11/DrawAbleManager.h>
# include <MyDx11/DrawAble/DrawAbleInterface.h>
# include <d3d11.h>
# include <typeinfo>
# include <map>
# include <string>


ZDSJ::DrawAbleManager::DrawAbleManager(RenderType _render_type) : m_render_type(_render_type)
{
	switch (this->m_render_type)
	{
	case Default:
		this->m_container = new std::vector<ZDSJ::DrawAbleInterface*>;
		break;
	case Category:
		this->m_container = new std::map<std::string, std::vector<ZDSJ::DrawAbleInterface*>>;
		break;
	default:
		break;
	}
}

ZDSJ::DrawAbleManager::~DrawAbleManager()
{
	switch (this->m_render_type)
	{
	case Default:
		this->vectorContainerClear();
		break;
	case Category:
		this->mapContainerClear();
		break;
	default:
		break;
	}
	delete this->m_container;
}

void ZDSJ::DrawAbleManager::add(ZDSJ::DrawAbleInterface* _drawable)
{
	switch (this->m_render_type)
	{
	case Default:
		this->vectorContainerAdd(_drawable);
		break;
	case Category:
		this->mapContainerAdd(_drawable);
		break;
	default:
		break;
	}
}

void ZDSJ::DrawAbleManager::render(ID3D11DeviceContext* _context)
{
	switch (this->m_render_type)
	{
	case Default:
		this->defaultRender(_context);
		break;
	case Category:
		this->CategoryRender(_context);
		break;
	default:
		break;
	}
}

void ZDSJ::DrawAbleManager::vectorContainerClear()
{
	std::vector<ZDSJ::DrawAbleInterface*>* container = reinterpret_cast<std::vector<ZDSJ::DrawAbleInterface*>*>(this->m_container);
	for (auto item : *container) {
		delete item;
	}
}

void ZDSJ::DrawAbleManager::mapContainerClear()
{
	std::map<std::string, std::vector<ZDSJ::DrawAbleInterface*>>* container = reinterpret_cast<std::map<std::string, std::vector<ZDSJ::DrawAbleInterface*>>*>(this->m_container);
	for (auto pair : *container) {
		for (auto item : pair.second) {
			delete item;
		}
	}
}

void ZDSJ::DrawAbleManager::vectorContainerAdd(ZDSJ::DrawAbleInterface* _drawable)
{
	std::vector<ZDSJ::DrawAbleInterface*>* container = reinterpret_cast<std::vector<ZDSJ::DrawAbleInterface*>*>(this->m_container);
	container->push_back(_drawable);
}

void ZDSJ::DrawAbleManager::defaultRender(ID3D11DeviceContext* _context)
{
	std::vector<ZDSJ::DrawAbleInterface*>* container = reinterpret_cast<std::vector<ZDSJ::DrawAbleInterface*>*>(this->m_container);
	for (auto item : *container) {
		item->draw(_context);
	}
}

void ZDSJ::DrawAbleManager::CategoryRender(ID3D11DeviceContext* _context)
{
	std::map<std::string, std::vector<ZDSJ::DrawAbleInterface*>>* container = reinterpret_cast<std::map<std::string, std::vector<ZDSJ::DrawAbleInterface*>>*>(this->m_container);
	for (auto pair : *container) {
		pair.second.at(0)->bindStatic(_context);
		for (auto item : pair.second) {
			item->update(_context);
			item->bind(_context);
			item->drawIndex(_context, 0u, 0u);
		}
	}
}

void ZDSJ::DrawAbleManager::mapContainerAdd(ZDSJ::DrawAbleInterface* _drawable)
{
	std::map<std::string, std::vector<ZDSJ::DrawAbleInterface*>>* container = reinterpret_cast<std::map<std::string, std::vector<ZDSJ::DrawAbleInterface*>>*>(this->m_container);
	std::string type_name(typeid(*_drawable).name());
	std::map<std::string, std::vector<ZDSJ::DrawAbleInterface*>>::iterator item = container->end();
	do {
		item = container->find(type_name);
		if (item != container->end()) {
			item->second.push_back(_drawable);
		}
		else {
			container->emplace(std::make_pair(type_name, std::vector<ZDSJ::DrawAbleInterface*>()));
		}
	} while (item == container->end());
}

