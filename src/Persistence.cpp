# include <Persistence.h>
# include <MyDx11/DrawAbleManager.h>
# include <MyDx11/DrawAble/DrawAbleAdapter.h>

ZDSJ::Persistence::Persistence(std::string _filename, bool _out)
{
	if (_out) {
		this->m_output_stream = new std::ofstream(_filename, std::ios::out | std::ios::trunc);
	}
	else {
		this->m_input_stream = new std::ifstream(_filename, std::ios::in);
	}
}

bool ZDSJ::Persistence::load(ID3D11Device* _device, ID3D11DeviceContext* _context, DrawAbleManager*& _manager)
{
	if (this->m_input_stream == nullptr) {
		return false;
	}
	if (!this->m_input_stream->is_open()) {
		return false;
	}
	size_t size = 1024;
	size_t offset = 0;
	char* buffer = new char[size];
	this->m_input_stream->read(buffer, size);
	ZDSJ::RenderType render_type = *(reinterpret_cast<ZDSJ::RenderType*>(buffer + offset));
	offset += sizeof(render_type);
	_manager = new ZDSJ::DrawAbleManager(render_type);
	ZDSJ::DrawAbleInterface* node = nullptr;
	do {
		node = ZDSJ::DrawAbleAdapter::load(_device, _context, buffer, offset, size);
		if (node != nullptr) {
			_manager->add(node);
		}
	} while (node != nullptr);
	return true;
}

bool ZDSJ::Persistence::save(const DrawAbleManager* _manager)
{
	if (this->m_output_stream == nullptr) {
		return false;
	}
	// 添加type
	this->m_output_stream->write(reinterpret_cast<const char*>(&_manager->m_render_type), sizeof(ZDSJ::RenderType));
	size_t size = 0;
	char* data = nullptr;
	data = _manager->save(size);
	if (data != nullptr) {
		this->m_output_stream->write(data, size);
		delete[] data;
	}
	return true;
}

bool ZDSJ::Persistence::save(const uint8_t* _data, const size_t _size) const
{
	if(this->m_output_stream == nullptr)
	{
		return false;
	}
	this->m_output_stream->write(reinterpret_cast<const char*>(_data), _size);

	return true;
}

bool ZDSJ::Persistence::load(uint8_t* _data, const size_t _size) const
{
	if(this->m_input_stream == nullptr)
	{
		return false;
	}
	this->m_input_stream->read(reinterpret_cast<char*>(_data), _size);
	return true;
}

ZDSJ::Persistence::~Persistence()
{
	if (this->m_output_stream != nullptr) {
		this->m_output_stream->close();
		delete this->m_output_stream;
	}

	if (this->m_input_stream != nullptr) {
		this->m_input_stream->close();
		delete this->m_input_stream;
	}
}
