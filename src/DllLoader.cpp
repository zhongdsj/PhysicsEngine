# include <DllLoader.h>

ZDSJ::DllLoader::DllLoader()
{
	this->m_indispensable.push_back(L"Common.dll");
	this->m_indispensable.push_back(L"MainWindowUi.dll");
}

bool ZDSJ::DllLoader::load()
{
	// 尝试加载必要DLL
	bool indispensable_loaded = true;
	for (auto indispensable : this->m_indispensable)
	{
		if(DllLoader::isLoaded(indispensable))
		{
			continue;
		}
		HINSTANCE temp = LoadLibrary(indispensable);
		if (temp == nullptr)
		{
			indispensable_loaded = false;
			break;
		}
		this->m_dlls.push_back(temp);
	}
	if(indispensable_loaded)
	{
		return indispensable_loaded;
	}
	// todo 后续插件加载
	return indispensable_loaded;
}

ZDSJ::DllLoader::~DllLoader()
{
	for(auto item : this->m_dlls)
	{
		// 移除dll
		FreeLibrary(item);
	}
}

bool ZDSJ::DllLoader::isLoaded(const wchar_t* dll_name)
{
	HINSTANCE instance = GetModuleHandle(dll_name);
	return (instance != nullptr);
}
