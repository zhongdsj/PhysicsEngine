# pragma once
# include <MyDx11/DrawAble/DrawAbleAdapter.h>
# include <MyDx11/BindAble/BindAbleInterface.h>
# include <MyDx11/BindAble/IndexBufferBindAble.h>
# include <MyDx11/BindAble/VertexBufferBindAble.h>
# include <MyDx11/VertexStructure.h>

namespace ZDSJ {
	// struct Vertex2D;
	// class VertexBufferBindAble;
	

	template<class T>
	class DrawAbleBase : public DrawAbleAdapter {
	public:
		DrawAbleBase(const DrawAbleData& _data): DrawAbleAdapter(_data) {
			this->m_object_count++;
		}

		bool isStaticInitialized() const {
			return !this->m_static_bindable.empty();
		}

		void addStaticBind(BindAbleInterface* _bindable) {
			this->m_static_bindable.push_back(_bindable);
		}

		void addStaticIndexBuffer(IndexBufferBindAble* _index_buffer, size_t _index_size) {
			this->m_index_size = _index_size;
			this->m_static_bindable.push_back(_index_buffer);
		}

		void setVertexBindAble(ID3D11Device* _device, ID3D11DeviceContext* _context, VertexBufferBindAble* _vertex_bindable) {
			this->m_static_color_vertex = _vertex_bindable;
			std::vector<ZDSJ::Vertex2D> vertices;
			ZDSJ::Vertex2D* vertex_buffer = nullptr;
			size_t vertex_buffer_size = 0;
			_vertex_bindable->getData(_device, _context, reinterpret_cast<void**>(&vertex_buffer), vertex_buffer_size);
			vertex_buffer_size /= sizeof(ZDSJ::Vertex2D);
			for (int i = 0; i < vertex_buffer_size; ++i) {
				vertex_buffer[i].color = { 255, 255, 255, 255 };
				vertices.push_back(vertex_buffer[i]);
			}
			delete[] vertex_buffer;
			this->m_static_border_vertex = new ZDSJ::VertexBufferBindAble(_device, vertices);
		}
		~DrawAbleBase() {
			this->m_object_count--;
			if (this->m_object_count == 0) {
				for (auto item : this->m_static_bindable) {
					delete item;
				}
				this->m_static_bindable.clear();
				delete this->m_static_color_vertex;
				delete this->m_static_border_vertex;
			}
		}
	private:
		static std::vector<BindAbleInterface*> m_static_bindable;
		static size_t m_index_size;
		static BindAbleInterface* m_static_color_vertex;
		static BindAbleInterface* m_static_border_vertex;
		static int m_object_count;
	protected:
		const std::vector<BindAbleInterface*>& getStaticBindAble() const override {
			return this->m_static_bindable;
		}

		size_t getStaticIndexSize() const override {
			return this->m_index_size;
		}

		BindAbleInterface* getColorVertex() const override {
			return this->m_static_color_vertex;
		}

		BindAbleInterface* getBorderVertex() const override {
			return m_static_border_vertex;
		}

		bool getStaticVertexBufferAndIndexBuffer(const VertexBufferBindAble*& _vertex_buffer_bindable, const IndexBufferBindAble*& _index_buffer_bindable) const {
			bool result = false;
			_vertex_buffer_bindable = reinterpret_cast<VertexBufferBindAble*>(this->m_static_color_vertex);
			for (auto item : this->m_static_bindable) {
				if (const auto ptr = dynamic_cast<IndexBufferBindAble*>(item)) {
					_index_buffer_bindable = ptr;
				}
				if (_index_buffer_bindable != nullptr) {
					result = true;
					break;
				}
			}
			return result;
		}
	};

	template<class T>
	std::vector<BindAbleInterface*> DrawAbleBase<T>::m_static_bindable;
	template<class T>
	size_t DrawAbleBase<T>::m_index_size = 0;
	template<class T>
	BindAbleInterface* DrawAbleBase<T>::m_static_color_vertex = nullptr;
	template<class T>
	BindAbleInterface* DrawAbleBase<T>::m_static_border_vertex = nullptr;
	template<class T>
	int DrawAbleBase<T>::m_object_count = 0;

}