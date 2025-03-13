# pragma once

namespace ZDSJ
{
	class ComponentInterface
	{
	public:
		virtual void tick(float _use_time) = 0;
		//virtual LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		virtual ~ComponentInterface() = default;
	};
}
