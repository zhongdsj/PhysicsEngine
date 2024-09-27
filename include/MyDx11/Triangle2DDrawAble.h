# pragma once
# include <MyDx11/DrawAbleAdapter.h>

struct ID3D11Buffer;

namespace ZDSJ {
	class BindAbleInterface;

	class Triangle2DDrawAble : public DrawAbleAdapter {
	public:
		Triangle2DDrawAble(ID3D11Device* _device, ID3D11DeviceContext* _context);
		~Triangle2DDrawAble() = default;
	protected:
		void update(ID3D11DeviceContext* _context, short _fps) override;
	private:
		ID3D11Buffer* m_buffer = nullptr;
		short m_run_fps = 0;
		short m_max_fps = 4.0f * 60;
	};
}