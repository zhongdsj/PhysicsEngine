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
		float3 size;
		float3 pos;
		float3 rotation;
		DrawAbleData(): DrawAbleData(float3({ 100.0f, 100.0f, 1.0f }), float3({ 0.0f, 0.0f, 0.0f }), float3({ 0.0f, 0.0f, 0.0f })){};
		DrawAbleData(float3 _size, float3 _pos, float3 _rotation):size(_size), pos(_pos), rotation(_rotation){}
	};

	class DrawAbleAdapter : public DrawAbleInterface {
		
	public:
		DrawAbleAdapter(const DrawAbleData& _data = DrawAbleData());
		void draw(ID3D11DeviceContext* _context) override;
		const DrawAbleData* getData() const;
		
		inline DrawAbleAdapter* setSizeX(float _value) { this->m_data->size.x = _value; return this; }
		inline DrawAbleAdapter* setSizeY(float _value) { this->m_data->size.y = _value; return this;}
		inline DrawAbleAdapter* setSizeZ(float _value) { this->m_data->size.z = _value; return this;}
		inline DrawAbleAdapter* setPosX(float _value) { this->m_data->pos.x = _value; return this;}
		inline DrawAbleAdapter* setPosY(float _value) { this->m_data->pos.y = _value; return this;}
		inline DrawAbleAdapter* setPosZ(float _value) { this->m_data->pos.z = _value; return this;}
		inline DrawAbleAdapter* setRotationX(float _value) { this->m_data->rotation.x = _value; return this;}
		inline DrawAbleAdapter* setRotationY(float _value) { this->m_data->rotation.y = _value; return this;}
		inline DrawAbleAdapter* setRotationZ(float _value) { this->m_data->rotation.z = _value; return this;}

		inline DrawAbleAdapter* addAnimation(ZDSJ::DrawAbleAnimation* _animation) { this->m_animation.push_back(_animation); return this; };
		~DrawAbleAdapter();
	protected:
		std::vector<BindAbleInterface*>* m_bind_able = nullptr;
		ConstantBufferBindAble* m_transform = nullptr;
		void update(ID3D11DeviceContext* _context) override;
		void bind(ID3D11DeviceContext* _context) override;
		void bindStatic(ID3D11DeviceContext* _context) override;
		void drawIndex(ID3D11DeviceContext* _context, unsigned int _start_index_location = 0u, int _base_vertex_location = 0u) override;
		DirectX::XMMATRIX getTransformMatix() const;

		std::vector<ZDSJ::DrawAbleAnimation*> m_animation;
		DrawAbleData* m_data = nullptr;

		virtual const std::vector<BindAbleInterface*>& getStaticBindAble() const = 0;
		virtual const size_t getStaticIndexSize() const = 0;
	};
}