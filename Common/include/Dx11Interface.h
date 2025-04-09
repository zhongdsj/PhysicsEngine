# pragma once
# include <ComponentInterface.h>
# include <CommonExport.h>
# include <wtypes.h>
# include <CommonMacro.h>

struct ID3D11Device;
struct ID3D11DeviceContext;

namespace ZDSJ
{
	class Common_Api Dx11Interface : public ComponentInterface
	{
	public:
		Dx11Interface() = default;
		Delete_Copy_And_Move(Dx11Interface);
		virtual ID3D11Device* device() const = 0;
		virtual ID3D11DeviceContext* context() const = 0;
		virtual void beginTick() = 0;
		virtual void endTick() = 0;
		virtual void solid() = 0;
		virtual void wireframe() = 0;
		virtual ~Dx11Interface() override = default;
	protected:
		void registerToContext();
	};
	Common_Api ZDSJ::Dx11Interface* createDx11(HWND _hwnd, int _window_width, int _window_height, int _render_type);
}
