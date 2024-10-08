# pragma once
# include <MyDx11/BindAble/BindAbleInterface.h>

struct ID3D11PixelShader;

namespace ZDSJ {

	class PixelShaderBindAble : public BindAbleInterface {

	public:
		PixelShaderBindAble(ID3D11Device* _device, const BYTE* _pixel_shader, size_t _pixel_shader_size);
		void bind(ID3D11DeviceContext* _context) override;
		~PixelShaderBindAble() = default;
	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pixelShader = nullptr;
	};

}