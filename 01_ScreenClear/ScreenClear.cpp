#include "pch.h"
#include "ScreenClear.h"

ScreenClear::ScreenClear(const int32& width, const int32& height, const std::wstring& name)
	: GameProcessor(width, height, name)
{
	
}

void ScreenClear::Initialize()
{
	GameProcessor::CreateDeviceAndSwapChain();
	GameProcessor::CreateRenderTargetView();
	GameProcessor::SetViewport();
}

void ScreenClear::Update()
{
	GameProcessor::Update();
}

void ScreenClear::Render()
{
	GameProcessor::RenderBegin();

	// TO DO

	GameProcessor::RenderEnd();
}
