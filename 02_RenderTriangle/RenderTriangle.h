#pragma once
#include "GameProcessor.h"

class RenderTriangle :
	public GameProcessor
{
public:
	RenderTriangle(const int32& width, const int32& height, const std::wstring& name);
	virtual ~RenderTriangle() = default;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void CreateGeometry() override;
};

