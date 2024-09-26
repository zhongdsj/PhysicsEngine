# pragma once
# include <MyDx11/DrawAbleInterface.h>

namespace ZDSJ {
	class BindAbleInterface;

	class DrawAbleAdapter : public DrawAbleInterface {
	public:
		DrawAbleAdapter(ID3D11Device* _device);
		void draw(ID3D11DeviceContext* _context) override;
		~DrawAbleAdapter();
	protected:
		size_t m_index_size = 0;
		std::vector<BindAbleInterface*>* m_bind_able = nullptr;

		void bind(ID3D11DeviceContext* _context);
	};
}