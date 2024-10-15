# pragma once
# include <MyDx11/DrawAble/DrawAbleAdapter.h>
# include <MyDx11/BindAble/BindAbleInterface.h>
# include <MyDx11/BindAble/IndexBufferBindAble.h>

namespace ZDSJ {

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
	};

	template<class T>
	std::vector<BindAbleInterface*> DrawAbleBase<T>::m_static_bindable;
	template<class T>
	size_t DrawAbleBase<T>::m_index_size = 0;

}