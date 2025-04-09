# include <MyDx11/DrawAbleManager.h>
# include <MyDx11/DrawAble/DrawAbleInterface.h>
# include <d3d11.h>
# include <typeinfo>
# include <map>
# include <sstream>
# include <string>
# include <MyDx11/VertexStructure.h>
# include <Context.h>


ZDSJ::DrawAbleManager::DrawAbleManager(const RenderType _render_type) : m_render_type(_render_type)
{
	switch (this->m_render_type)
	{
	case ZDSJ::RenderType::Default:
		this->m_container = new std::vector<ZDSJ::DrawAbleInterface*>;
		break;
	case ZDSJ::RenderType::Category:
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
	case ZDSJ::RenderType::Default:
		this->vectorContainerClear();
		break;
	case ZDSJ::RenderType::Category:
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
	case ZDSJ::RenderType::Default:
		this->vectorContainerAdd(_drawable);
		break;
	case ZDSJ::RenderType::Category:
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
	case ZDSJ::RenderType::Default:
		this->defaultRender(_context);
		break;
	case ZDSJ::RenderType::Category:
		this->categoryRender(_context);
		break;
	default:
		break;
	}
}

char* ZDSJ::DrawAbleManager::save(size_t& _size) const
{
	char* data = nullptr;
	switch (this->m_render_type)
	{
	case ZDSJ::RenderType::Default:
		data = this->defaultSave(_size);
		break;
	case ZDSJ::RenderType::Category:
		data = this->categorySave(_size);
		break;
	default:
		break;
	}
	return data;
}

void ZDSJ::DrawAbleManager::vectorContainerClear()
{
	const auto container = static_cast<std::vector<ZDSJ::DrawAbleInterface*>*>(this->m_container);
	for (const auto item : *container) {
		delete item;
	}
}

void ZDSJ::DrawAbleManager::mapContainerClear()
{
	const auto container = static_cast<std::map<std::string, std::vector<ZDSJ::DrawAbleInterface*>>*>(this->m_container);
	for (auto pair : *container) {
		for (const auto item : pair.second) {
			delete item;
		}
	}
}

void ZDSJ::DrawAbleManager::vectorContainerAdd(ZDSJ::DrawAbleInterface* _drawable)
{
	const auto container = static_cast<std::vector<ZDSJ::DrawAbleInterface*>*>(this->m_container);
	container->push_back(_drawable);
}

void ZDSJ::DrawAbleManager::mapContainerAdd(ZDSJ::DrawAbleInterface* _drawable)
{
	const auto container = static_cast<std::map<std::string, std::vector<ZDSJ::DrawAbleInterface*>>*>(this->m_container);
	std::string type_name(typeid(*_drawable).name());
	auto item = container->end();
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


void ZDSJ::DrawAbleManager::itemSave(ZDSJ::DrawAbleInterface* _item, std::ostringstream* _oss, size_t& _size) const
{
	char* temp = nullptr;
	size_t size = 0;
	temp = _item->save(size);
	_size += size;
	if (temp != nullptr) {
		_oss->write(temp, size);
		delete[] temp;
	}
}
char* ZDSJ::DrawAbleManager::defaultSave(size_t& _size) const
{
	std::ostringstream oss;
	const auto container = static_cast<std::vector<ZDSJ::DrawAbleInterface*>*>(this->m_container);
	for (const auto item : *container) {
		this->itemSave(item, &oss, _size);
	}
	char* result = nullptr;
	result = new char[_size];
	memcpy_s(result, _size, oss.str().data(), _size);
	return result;
}

char* ZDSJ::DrawAbleManager::categorySave(size_t& _size) const
{
	std::ostringstream oss;
	std::map<std::string, std::vector<ZDSJ::DrawAbleInterface*>>* container = reinterpret_cast<std::map<std::string, std::vector<ZDSJ::DrawAbleInterface*>>*>(this->m_container);
	for (auto pair : *container) {
		for (auto item : pair.second) {
			this->itemSave(item, &oss, _size);
		}
	}
	char* result = nullptr;
	result = new char[_size];
	memcpy_s(result, _size, oss.str().data(), _size);
	return result;
}

void ZDSJ::DrawAbleManager::itemRender(std::vector<ZDSJ::DrawAbleInterface*>::iterator& _iterator, ID3D11DeviceContext* _context, std::vector<ZDSJ::DrawAbleInterface*>& _container, const ZDSJ::Point& _mouse_word, bool _draw_static)
{
	if(ZDSJ::Context::getInstance()->getWordActive())
	{
		// 活动
		// 计算受力
		auto next = _iterator + 1;
		while (next != _iterator)
		{
			if (next == _container.end())
			{
				next = _container.begin();
			}
			if (next == _iterator)
			{
				break;
			}
			(*_iterator)->calculateForce(*next);
			++next;
		}
		// 计算加速度并移动
		// (*_iterator)->moveByAcceleration();
	}

	//if ((*_iterator)->pointInPolgon2D(_mouse_word.x, _mouse_word.y)) {
	//	// hover
	//	if (ZDSJ::Context::getInstance()->mouseClick()) {
	//		// click 仅绘制边框
	//		(*_iterator)->click();
	//	}
	//	if (ZDSJ::Context::getInstance()->mouseRight()) {
	//		const auto temp = _iterator;
	//		delete (*temp);
	//		_iterator = _container.erase(temp);
	//		return;
	//	}
	//}
	(*_iterator)->draw(_context, _draw_static);
	++_iterator;
}

void ZDSJ::DrawAbleManager::defaultRender(ID3D11DeviceContext* _context)
{
	ZDSJ::Point mouse_word = Context_Instance->getMouseWorld();
	std::vector<ZDSJ::DrawAbleInterface*>* container = static_cast<std::vector<ZDSJ::DrawAbleInterface*>*>(this->m_container);
	std::vector<ZDSJ::DrawAbleInterface*>::iterator item = container->begin();
	while (item != container->end()) {
		this->itemRender(item, _context, *container, mouse_word, true);
	}
}

void ZDSJ::DrawAbleManager::categoryRender(ID3D11DeviceContext* _context)
{
	ZDSJ::Point mouse_word = Context_Instance->getMouseWorld();
	std::map<std::string, std::vector<ZDSJ::DrawAbleInterface*>>* container = static_cast<std::map<std::string, std::vector<ZDSJ::DrawAbleInterface*>>*>(this->m_container);
	auto pair = container->begin();
	std::vector<ZDSJ::DrawAbleInterface*>::iterator item;
	while (pair != container->end()) {
		if (pair->second.size() > 0) {
			pair->second.at(0)->bindStatic(_context);
		}
		item = pair->second.begin();
		while (item != pair->second.end()) {
			this->itemRender(item, _context, pair->second, mouse_word, false);
		}
		++pair;
	}
}

