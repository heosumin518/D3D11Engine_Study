#include "pch.h"
#include "Game.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

WPARAM Engine::Game::Run(GameDesc& desc)
{
	m_desc = desc;
	assert(m_desc.app != nullptr);

	// 윈도우 창 정보 등록
	MyRegisterClass();

	// 윈도우 창 생성
	if (!InitInstance(SW_SHOWNORMAL))
		return FALSE;

	GRAPHICS->Init(m_desc.hWnd);
	TIME->Init();


	m_desc.app->Init();

	MSG msg = { 0 };

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			Update();
		}
	}

	return msg.wParam;
}

ATOM Engine::Game::MyRegisterClass()
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_desc.hInstance;
	wcex.hIcon = ::LoadIcon(NULL, IDI_WINLOGO);
	wcex.hCursor = ::LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = m_desc.appName.c_str();
	wcex.hIconSm = wcex.hIcon;

	return RegisterClassExW(&wcex);
}

BOOL Engine::Game::InitInstance(int cmdShow)
{
	RECT windowRect = RECT{ 0, 0, static_cast<LONG>(m_desc.width), static_cast<LONG>(m_desc.height) };
	::AdjustWindowRect(&windowRect, WS_OVERLAPPEDWINDOW, false);

	m_desc.hWnd = CreateWindowW(
		m_desc.appName.c_str(), m_desc.appName.c_str(),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,
		windowRect.right - windowRect.left, windowRect.bottom - windowRect.top,
		nullptr, nullptr, 
		m_desc.hInstance,
		nullptr
		);

	if (m_desc.hWnd)
		return FALSE;

	::ShowWindow(m_desc.hWnd, cmdShow);
	::UpdateWindow(m_desc.hWnd);

	return TRUE;
}

void Engine::Game::Update()
{
	// 매니저 등등 업데이트

	TIME->Update();

	GRAPHICS->RenderBegin();

#ifdef _DEBUG
	ShowFPS();
#endif

	m_desc.app->Update();
	m_desc.app->Render();

	GRAPHICS->RenderEnd();
}

void Engine::Game::ShowFPS()
{
	uint32_t fps = GET_SINGLE(TimeManager)->GetFPS();

	WCHAR text[100] = L"";
	::wsprintf(text, L"FPS : %d", fps);

	::SetWindowText(m_desc.hWnd, text);
}

LRESULT CALLBACK Engine::Game::WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(handle, message, wParam, lParam))
		return true;

	switch (message)
	{
	case WM_SIZE:
		break;
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return ::DefWindowProc(handle, message, wParam, lParam);
	}
}
