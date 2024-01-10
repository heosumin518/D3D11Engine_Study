#pragma once

namespace Engine
{
	struct GameDesc
	{
		shared_ptr<class IExecute> app = nullptr;
		wstring appName = L"SampleGameName";
		HINSTANCE hInstance = 0;
		HWND hWnd = 0;
		float width = 800;
		float height = 600;
		bool vsync = false;
		bool windowed = true;
		Color clearColor = Color(0.5f, 0.5f, 0.5f);
	};

	class Game
	{
		DECLARE_SINGLE(Game);
	public:
		WPARAM Run(GameDesc& desc);

		GameDesc& GetGameDesc() { return m_desc; }

	private:
		ATOM MyRegisterClass();
		BOOL InitInstance(int cmdShow);

		void Update();
		void ShowFPS();

		static LRESULT CALLBACK WndProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		GameDesc m_desc;
	};
}



