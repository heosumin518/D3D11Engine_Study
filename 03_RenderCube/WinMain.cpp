#include "pch.h"
#include "RenderCube.h"
#include "WinApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	RenderCube renderCube(g_winSizeX, g_winSizeY, g_gameName);
	return WinApp::Run(&renderCube, hInstance, nCmdShow);
}