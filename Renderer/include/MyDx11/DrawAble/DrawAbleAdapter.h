# pragma once
# include <vector>
# include <MyDx11/DrawAble/DrawAbleInterface.h>

namespace DirectX {
	struct XMMATRIX;
}

namespace ZDSJ {
	class BindAbleInterface;
	class ConstantBufferBindAble;
	class VertexBufferBindAble;
	class IndexBufferBindAble;

	enum class DrawAbleClass {
		Default,
		Triangle2D,
		Rectangle2D,
		Arc2D,
	};

	enum class DrawAbleState {
		Default = 0,
		Hover = 1,
		Check = 2,
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
		void draw(ID3D11DeviceContext* _context, bool _bind_static = true) override;
		const float3& position() const override;
		const float3& size() const override;
		MovementInterface* getMovement() const override;
		void calculateForce(DrawAbleInterface* _other) override;
		void moveByAcceleration() override;
		bool pointInPolgon2D(float _x, float _y) override;
		char* save(size_t& _size) override;
		static DrawAbleInterface* load(ID3D11Device* _device, ID3D11DeviceContext* _context, const char* _data, size_t& _offset, size_t _size);

		inline void click() override { this->hasState(ZDSJ::DrawAbleState::Check) ? this->removeState(ZDSJ::DrawAbleState::Check) : this->addState(ZDSJ::DrawAbleState::Check); };
		inline void addState(DrawAbleState _state){ this->m_state = DrawAbleAdapter::addState(this->m_state, _state); }
		inline void removeState(DrawAbleState _state){ this->m_state = DrawAbleAdapter::removeState(this->m_state, _state); }
		inline bool hasState(DrawAbleState _state) { return DrawAbleAdapter::hasState(this->m_state, _state); }
		inline DrawAbleAdapter* setSizeX(float _value) { this->m_data->size.x = _value; return this; }
		inline DrawAbleAdapter* setSizeY(float _value) { this->m_data->size.y = _value; return this;}
		inline DrawAbleAdapter* setSizeZ(float _value) { this->m_data->size.z = _value; return this;}
		inline DrawAbleAdapter* setPosX(float _value) { this->m_data->pos.x = _value; return this;}
		inline DrawAbleAdapter* setPosY(float _value) { this->m_data->pos.y = _value; return this;}
		inline DrawAbleAdapter* setPosZ(float _value) { this->m_data->pos.z = _value; return this;}
		inline DrawAbleAdapter* setRotationX(float _value) { this->m_data->rotation.x = _value; return this;}
		inline DrawAbleAdapter* setRotationY(float _value) { this->m_data->rotation.y = _value; return this;}
		inline DrawAbleAdapter* setRotationZ(float _value) { this->m_data->rotation.z = _value; return this;}

		~DrawAbleAdapter() override;
	protected:
		std::vector<BindAbleInterface*>* m_bind_able = nullptr;
		ConstantBufferBindAble* m_transform = nullptr;
		void update(ID3D11DeviceContext* _context) override;
		void bind(ID3D11DeviceContext* _context) override;
		void bindStatic(ID3D11DeviceContext* _context) override;
		void drawIndex(ID3D11DeviceContext* _context, unsigned int _start_index_location = 0u, int _base_vertex_location = 0u) override;
		DirectX::XMMATRIX getTransformMatix() const;

		MovementInterface* m_movement;
		DrawAbleData* m_data = nullptr;
		DrawAbleClass m_drawable_class = ZDSJ::DrawAbleClass::Default;
		std::vector<Vertex2D> m_vertices;
		std::vector<UINT16> m_indices;

		DrawAbleState m_state = DrawAbleState::Default;

		void drawBorder(ID3D11DeviceContext* _context);

		void setVertexBufferAndIndexBuffer(ID3D11Device* _device, ID3D11DeviceContext* _context, const VertexBufferBindAble*& _vertex_buffer_bindable, const IndexBufferBindAble*& _index_buffer_bindable);
		virtual const std::vector<BindAbleInterface*>& getStaticBindAble() const = 0;
		virtual size_t getStaticIndexSize() const = 0;
		virtual BindAbleInterface* getColorVertex() const = 0;
		virtual BindAbleInterface* getBorderVertex() const = 0;

		bool pointInTriangle2D(float _x, float _y, short _triangle_index, DirectX::XMMATRIX& _word_matrix) const;

		bool rayInTriangle2D(float _x, float _y, short _triangle_index, DirectX::XMMATRIX& _word_matrix) const;

		static inline bool hasState(DrawAbleState _combined, DrawAbleState _state) {
			return static_cast<unsigned int>(_combined) & static_cast<unsigned int>(_state);
		}

		// 辅助函数来添加状态到现有组合中  
		static inline DrawAbleState addState(DrawAbleState _combined, DrawAbleState _state) {
			return static_cast<DrawAbleState>(static_cast<unsigned int>(_combined) | static_cast<unsigned int>(_state));
		}

		// 辅助函数来移除状态  
		static inline DrawAbleState removeState(DrawAbleState _combined, DrawAbleState _state) {
			return static_cast<DrawAbleState>(static_cast<unsigned int>(_combined) & ~static_cast<unsigned int>(_state));
		}
	};
}