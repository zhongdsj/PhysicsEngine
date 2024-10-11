# pragma once
# include <MyDx11/DrawAble/DrawAbleInterface.h>
# include <MyDx11/Animations/DrawAbleAnimation.h>

namespace DirectX {
	struct XMMATRIX;
}

namespace ZDSJ {
	class BindAbleInterface;
	class ConstantBufferBindAble;

	struct float3 {
		float x;
		float y;
		float z;
	};

	struct DrawAbleData
	{
		float3 scale;
		float3 translation;
		float3 rotation;
		DrawAbleData(): DrawAbleData(float3({ 1.0f, 1.0f, 1.0f }), float3({ 0.0f, 0.0f, 0.0f }), float3({ 0.0f, 0.0f, 0.0f })){};
		DrawAbleData(float3 _scale, float3 _translation, float3 _rotation):scale(_scale), translation(_translation), rotation(_rotation){}
		// DrawAbleData(const DrawAbleData& _other):position(_other.position), translation(_other.translation), rotation(_other.rotation){}
	};

	class DrawAbleAdapter : public DrawAbleInterface {
	public:
		DrawAbleAdapter(ID3D11Device* _device, const DrawAbleData& _data = DrawAbleData());
		void draw(ID3D11DeviceContext* _context) override;
		const DrawAbleData* getData() const;
		
		inline DrawAbleAdapter* setScaleX(float _value) { this->m_data->scale.x = _value; return this; }
		inline DrawAbleAdapter* setScaleY(float _value) { this->m_data->scale.y = _value; return this;}
		inline DrawAbleAdapter* setScaleZ(float _value) { this->m_data->scale.z = _value; return this;}
		inline DrawAbleAdapter* setTranslationX(float _value) { this->m_data->translation.x = _value; return this;}
		inline DrawAbleAdapter* setTranslationY(float _value) { this->m_data->translation.y = _value; return this;}
		inline DrawAbleAdapter* setTranslationZ(float _value) { this->m_data->translation.z = _value; return this;}
		inline DrawAbleAdapter* setRotationX(float _value) { this->m_data->rotation.x = _value; return this;}
		inline DrawAbleAdapter* setRotationY(float _value) { this->m_data->rotation.y = _value; return this;}
		inline DrawAbleAdapter* setRotationZ(float _value) { this->m_data->rotation.z = _value; return this;}

		inline DrawAbleAdapter* addAnimation(ZDSJ::DrawAbleAnimation* _animation) { this->m_animation.push_back(_animation); return this; };
		~DrawAbleAdapter();
	protected:
		size_t m_index_size = 0;
		std::vector<BindAbleInterface*>* m_bind_able = nullptr;
		ConstantBufferBindAble* m_transform = nullptr;
		void update(ID3D11DeviceContext* _context) override;
		void bind(ID3D11DeviceContext* _context);
		DirectX::XMMATRIX getTransformMatix() const;

		std::vector<ZDSJ::DrawAbleAnimation*> m_animation;
		DrawAbleData* m_data = nullptr;
	};
}