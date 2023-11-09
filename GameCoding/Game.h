#pragma once

#include "GameObject.h"
#include "Pipeline.h"

class InputManager;
class TimeManager;
class SceneManager;
class ResourceManager;
class RenderManager;

class Game
{
public:
	Game();
	~Game();

public:
	void Init(HWND hwnd);
	void Update();
	void Render();

	shared_ptr<InputManager> GetInputManager() { return _input; }
	shared_ptr<TimeManager> GetTimeManager() { return _time; }
	shared_ptr<SceneManager> GetSceneManager() { return _scene; }
	shared_ptr<ResourceManager> GetResourceManager() { return _resource; }
	shared_ptr<RenderManager> GetRenderManager() { return _render; }

private:
	HWND _hwnd;

	shared_ptr<Graphics> _graphics;		// 전체에 하나만 있으면 되는 것

private:
	shared_ptr<InputManager> _input;
	shared_ptr<TimeManager> _time;
	shared_ptr<SceneManager> _scene;
	shared_ptr<ResourceManager> _resource;
	shared_ptr<RenderManager> _render;
};

// Game 을 어디서든 접근할 수 있게 전역 변수를 만들어 놓았다.
// 이를 통해 매니저에 모두 접근 가능하게 하였음

extern unique_ptr<Game> GGame;