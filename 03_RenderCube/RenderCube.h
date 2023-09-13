#pragma once
#include "GameProcessor.h"

class RenderCube :
	public GameProcessor
{
public:
	RenderCube(const int32& width, const int32& height, const std::wstring& name);
	virtual ~RenderCube() = default;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;

	virtual void CreateGeometry() override;
	virtual void SetTransformMatrix() override;
};

