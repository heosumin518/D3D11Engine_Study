#include "pch.h"
#include "TexturedCubeLighting.h"
#include "WinApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	TexturedCubeLighting renderCube(900, 650, g_gameName);
	return WinApp::Run(&renderCube, hInstance, nCmdShow);
}