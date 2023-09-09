#pragma once

class GameProcessor;

class WinApp
{
public:
	static int Run(GameProcessor* gameProcessor, HINSTANCE hInstance, int nCmdShow);
	inline static HWND GetHandleWnd();

protected:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static HWND m_hWnd;
};


HWND WinApp::GetHandleWnd()
{
	return m_hWnd;
}
