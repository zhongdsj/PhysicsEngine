# pragma once
# include <vector>
# include <MyDx11/DrawAble/DrawAbleInterface.h>
# include<memory>

namespace DirectX {
	struct XMMATRIX;
}

namespace ZDSJ {
	class BindAbleInterface;
	class ConstantBufferBindAble;
	class VertexBufferBindAble;
	class IndexBufferBindAble;

	enum class DrawAbleState {
		Default = 0,
		Hover = 1,
		Check = 2,
	};

	class DrawAbleAdapter : public DrawAbleInterface {
		
	public:
		DrawAbleAdapter();
		void draw(ID3D11DeviceContext* _context, const DrawAbleData* _drawAbleData, bool _bind_static = true) override;
		inline void click() override { this->hasState(ZDSJ::DrawAbleState::Check) ? this->removeState(ZDSJ::DrawAbleState::Check) : this->addState(ZDSJ::DrawAbleState::Check); }
		inline void addState(DrawAbleState _state){ this->m_state = DrawAbleAdapter::addState(this->m_state, _state); }
		inline void removeState(DrawAbleState _state){ this->m_state = DrawAbleAdapter::removeState(this->m_state, _state); }
		inline bool hasState(DrawAbleState _state) { return DrawAbleAdapter::hasState(this->m_state, _state); }

		~DrawAbleAdapter() override;
	protected:
		std::vector<std::shared_ptr<BindAbleInterface>>* m_bind_able = nullptr;
		std::shared_ptr<ConstantBufferBindAble> m_transform = nullptr;
		void update(ID3D11DeviceContext* _context, const DrawAbleData* _drawAbleData) override;
		void bind(ID3D11DeviceContext* _context) override;
		void bindStatic(ID3D11DeviceContext* _context) override;
		void drawIndex(ID3D11DeviceContext* _context, unsigned int _start_index_location = 0u, int _base_vertex_location = 0u) override;
		DirectX::XMMATRIX getTransformMatix(const DrawAbleData* _drawAbleData) const;

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