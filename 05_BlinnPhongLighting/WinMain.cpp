#include "pch.h"
#include "BlinnPhongLighting.h"
#include "WinApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	BlinnPhongLighting blinnPhongLighting(1280, 1000, g_gameName);
	return WinApp::Run(&blinnPhongLighting, hInstance, nCmdShow);
}