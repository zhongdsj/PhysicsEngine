# pragma once
# include <MyDx11/DrawAble/DrawAbleAdapter.h>
# include <MyDx11/BindAble/BindAbleInterface.h>
# include <MyDx11/BindAble/IndexBufferBindAble.h>

namespace ZDSJ {
	class VertexBufferBindAble;

	template<class T>
	class DrawAbleBase : public DrawAbleAdapter {
	public:
		DrawAbleBase(const DrawAbleData& _data): DrawAbleAdapter(_data) {

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

	private:
		static std::vector<BindAbleInterface*> m_static_bindable;
		static size_t m_index_size;
	protected:
		const std::vector<BindAbleInterface*>& getStaticBindAble() const override {
			return this->m_static_bindable;
		}

		const size_t getStaticIndexSize() const override {
			return this->m_index_size;
		}

		bool getStaticVertexBufferAndIndexBuffer(const VertexBufferBindAble*& _vertex_buffer_bindable, const IndexBufferBindAble*& _index_buffer_bindable) const {
			bool result = false;
			for (auto item : this->m_static_bindable) {
				if (const auto ptr = dynamic_cast<VertexBufferBindAble*>(item)) {
					_vertex_buffer_bindable = ptr;
				}
				if (const auto ptr = dynamic_cast<IndexBufferBindAble*>(item)) {
					_index_buffer_bindable = ptr;
				}
				if (_vertex_buffer_bindable != nullptr && _index_buffer_bindable != nullptr) {
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

}