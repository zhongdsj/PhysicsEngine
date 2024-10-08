# pragma once
# include <MyDx11/BindAble/BindAbleInterface.h>
# include <vector>

struct ID3D11InputLayout;
struct D3D11_INPUT_ELEMENT_DESC;

namespace ZDSJ {

	class InputLayoutBindAble : public BindAbleInterface {
	public:
		InputLayoutBindAble(ID3D11Device* _device, std::vector<D3D11_INPUT_ELEMENT_DESC>& _ied, const BYTE* _vertex_shader, size_t _vertex_shader_size);
		void bind(ID3D11DeviceContext* _context) override;
		~InputLayoutBindAble() = default;
	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_inputLayout = nullptr;
	};

}