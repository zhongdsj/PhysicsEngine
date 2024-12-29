# include <iostream>
# include <Windows.h>
# include "Application.h"

int WINAPI WinMain(_In_ HINSTANCE _hInstance, _In_opt_ HINSTANCE _hPrevInstance, _In_ LPSTR _lpCmdLine, _In_ int _nShowCmd) {

	auto app = new ZDSJ::Application();
	app->run();
	delete app;
	return 0;
}