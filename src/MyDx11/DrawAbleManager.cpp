# include <MyDx11/DrawAbleManager.h>
# include <MyDx11/DrawAble/DrawAbleInterface.h>
# include <d3d11.h>
# include <typeinfo>
# include <map>
# include <string>
# include <MyDx11/VertexStructure.h>
# include <MyDx11/Context.h>


ZDSJ::DrawAbleManager::DrawAbleManager(RenderType _render_type) : m_render_type(_render_type)
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
	std::vector<ZDSJ::DrawAbleInterface*>* container = reinterpret_cast<std::vector<ZDSJ::DrawAbleInterface*>*>(this->m_container);
	for (auto item : *container) {
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

void ZDSJ::DrawAbleManager::itemRender(ZDSJ::DrawAbleInterface* _item, ID3D11DeviceContext* _context, const ZDSJ::Point& _mouse_word, bool _draw_static)
{
	if (_item->pointInPolgon2D(_mouse_word.x, _mouse_word.y) && ZDSJ::Context::getInstance()->mouseClick()) {
		_item->click();
	}
	_item->draw(_context, _draw_static);
}

void ZDSJ::DrawAbleManager::defaultRender(ID3D11DeviceContext* _context)
{
	ZDSJ::Point mouse_word = ZDSJ::Context::getInstance()->mouseWord();
	std::vector<ZDSJ::DrawAbleInterface*>* container = reinterpret_cast<std::vector<ZDSJ::DrawAbleInterface*>*>(this->m_container);
	for (auto item : *container) {
		this->itemRender(item, _context, mouse_word, true);
	}
}

void ZDSJ::DrawAbleManager::categoryRender(ID3D11DeviceContext* _context)
{
	ZDSJ::Point mouse_word = ZDSJ::Context::getInstance()->mouseWord();
	std::map<std::string, std::vector<ZDSJ::DrawAbleInterface*>>* container = reinterpret_cast<std::map<std::string, std::vector<ZDSJ::DrawAbleInterface*>>*>(this->m_container);
	for (auto pair : *container) {
		pair.second.at(0)->bindStatic(_context);
		for (auto item : pair.second) {
			this->itemRender(item, _context, mouse_word, false);
		}
	}
}

