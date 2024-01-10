#include "pch.h"
#include "WinApp.h"

#include "GameProcessor.h"

HWND WinApp::m_hWnd = nullptr;

int WinApp::Run(GameProcessor* gameProcessor, HINSTANCE hInstance, int nCmdShow)
{
	// 윈도우 창 정보 등록
	WNDCLASSEX windowClass = { 0 };
	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = WndProc;
	windowClass.hInstance = hInstance;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.lpszClassName = L"DefaultWindowClass";
	RegisterClassExW(&windowClass);

	// 윈도우 창 생성
	RECT windowRect = { 0, 0, static_cast<LONG>(gameProcessor->GetWidth()), static_cast<LONG>(gameProcessor->GetHeight()) };
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	m_hWnd = CreateWindowW(
		windowClass.lpszClassName,
		gameProcessor->GetTitle(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		nullptr,
		nullptr,
		hInstance,
		gameProcessor);

	gameProcessor->Initialize();
	ShowWindow(m_hWnd, nCmdShow);

	MSG msg = { nullptr, };
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			gameProcessor->Update();
			gameProcessor->Render();
		}
	}

	_CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
	_CrtDumpMemoryLeaks();

	return static_cast<int>(msg.wParam);
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WinApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}

	return 0;
}
