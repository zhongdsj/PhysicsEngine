# include <iostream>
# include <Windows.h>
# include <MyWindow.h>
# include <MyWindowClass.h>
# include <assimp/Importer.hpp>
# include <assimp/postprocess.h>
# include <assimp/scene.h>
# include <Timer.h>

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) {
	//Assimp::Importer importer;

	//// importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
	//std::string filename("./RotatingCube.3DS");
	//auto model = importer.ReadFile(filename.data(), 
	//	aiProcess_Triangulate // ��ֶ���Σ��������תΪ������
	//	| aiProcess_JoinIdenticalVertices // �ظ�ʹ�ö��㣬����Ƚ��Ͷ�������
	//);
	//
	//// ��ָ�� || δ��ȡ���� || û��������Ϣ
	//if (model == nullptr || (model->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !model->HasMeshes()) {
	//	return 1;
	//}

	ZDSJ::MyWindow* window = new ZDSJ::MyWindow();
	ZDSJ::MyWindowClass* window_class = new ZDSJ::MyWindowClass(GetModuleHandle(nullptr), L"ZDSJWindow");
	window_class->creatWindow(window, 200, 300, 800, 600, L"hello");
	MSG msg;
	BOOL ret;
	short fps = 60;
	ZDSJ::Timer* timer = new ZDSJ::Timer(fps);

	while (true) {
		timer->mark();
		ret = PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE);
		if (ret != 0)
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else {
				TranslateMessage(&msg);
				DispatchMessageW(&msg);
			}
		}
		timer->nextFps();
		window->doFrame();
	}
	delete window;
	delete window_class;
	return 0;
}