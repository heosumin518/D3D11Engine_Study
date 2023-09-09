#include "pch.h"
#include "ScreenClear.h"

ScreenClear::ScreenClear(const int32& width, const int32& height, const std::wstring& name)
	: GameProcessor(width, height, name)
{
	
}

void ScreenClear::Initialize()
{
	CreateDeviceAndSwapChain();
	CreateRenderTargetView();
	SetViewport();
}

void ScreenClear::Update()
{
	GameProcessor::Update();
}

void ScreenClear::Render()
{
	RenderBegin();

	// TO DO

	RenderEnd();
}
