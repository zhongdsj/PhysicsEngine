# pragma once
# include <MyDx11/DrawAbleInterface.h>

namespace DirectX {
	struct XMMATRIX;
}

namespace ZDSJ {
	class BindAbleInterface;
	class ConstantBufferBindAble;

	class DrawAbleAdapter : public DrawAbleInterface {
	public:
		DrawAbleAdapter(ID3D11Device* _device);
		void draw(ID3D11DeviceContext* _context, short _fps) override;
		~DrawAbleAdapter();
	protected:
		size_t m_index_size = 0;
		std::vector<BindAbleInterface*>* m_bind_able = nullptr;
		ConstantBufferBindAble* m_transform = nullptr;
		void update(ID3D11DeviceContext* _context, short _fps) override;
		void bind(ID3D11DeviceContext* _context);
		DirectX::XMMATRIX getTransformMatix() const;

		float m_scal_x = 1.0f;
		float m_scal_y = 1.0f;
		float m_scal_z = 1.0f;
		float m_translation_x = 0.0f;
		float m_translation_y = 0.0f;
		float m_translation_z = 0.0f;
		float m_rotation_x = 0.0f;
		float m_rotation_y = 0.0f;
		float m_rotation_z = 0.0f;
	};
}