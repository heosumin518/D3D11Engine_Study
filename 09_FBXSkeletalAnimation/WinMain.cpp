#include "pch.h"
#include "FBXSkeletalAnimation.h"
#include "WinApp.h"

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	FBXSkeletalAnimation fbxSkeletalAnimation(1600, 1200, g_gameName);
	return WinApp::Run(&fbxSkeletalAnimation, hInstance, nCmdShow);
}