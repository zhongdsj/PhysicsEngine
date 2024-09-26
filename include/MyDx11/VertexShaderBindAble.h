#pragma once
# include <MyDx11/BindAbleInterface.h>

struct ID3D11VertexShader;

namespace ZDSJ {

	class VertexShaderBindAble : public BindAbleInterface {
	public:
		VertexShaderBindAble(ID3D11Device* _device, const BYTE* _vertex_shader, size_t _vertex_shader_size);
		void bind(ID3D11DeviceContext* _context) override;
		~VertexShaderBindAble() = default;
	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_vertex_shader = nullptr;
	};

}