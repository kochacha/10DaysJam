//#define WIN32_LEAN_AND_MEAN
//#define _WINSOCK_DEPRECATED_NO_WARNINGS
//#include <winsock2.h>
#include <Windows.h>
#include "Application.h"
//#include <ws2tcpip.h>
//#pragma comment(lib, "ws2_32.lib")

//#include<Windows.h>
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdline, int nCmdShow)
{
	auto& app = KochaEngine::Application::Instance();
	if (!app.Initialize())
		return -1;
	app.Run();
	app.Terminate();
	return 0;
}