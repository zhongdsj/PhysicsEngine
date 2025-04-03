# include <iostream>
# include <Windows.h>
# include <Application.h>
# include <DllLoader.h>

#include "Logger.h"


int WINAPI WinMain(_In_ HINSTANCE _hInstance, _In_opt_ HINSTANCE _hPrevInstance, _In_ LPSTR _lpCmdLine, _In_ int _nShowCmd) {
	if (SetDllDirectory(L"dll\\") == 0) {
		std::cerr << "Failed to set DLL directory. Error code: " << GetLastError() << std::endl;
		return 1;
	}
	// 加载dll
	ZDSJ::DllLoader dll_loader;
	if(!dll_loader.load())
	{
		return -1;
	}
	auto app = new ZDSJ::Application();
	app->run();
	delete app;
	ZDSJ::Log_Info("exit engine");
	return 0;
}
