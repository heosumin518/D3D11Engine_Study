#include "pch.h"
#include "FBXTransformAnimation.h"
#include "WinApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	FBXTransformAnimation fbxTransformAnimation(1600, 1200, g_gameName);
	return WinApp::Run(&fbxTransformAnimation, hInstance, nCmdShow);
}