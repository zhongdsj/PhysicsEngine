# pragma once
# include <CommonMacro.h>
# include <vector>
# include <wtypes.h>

namespace ZDSJ
{
	class DllLoader
	{
	public:
		DllLoader();
		Delete_Copy_And_Move(DllLoader);
		bool load();

		~DllLoader();

	private:
		static bool isLoaded(const wchar_t* dll_name);
		std::vector<const wchar_t*> m_indispensable;
		std::vector<HINSTANCE> m_dlls;
	};
}
