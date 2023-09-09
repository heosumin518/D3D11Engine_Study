#pragma once
#include "GameProcessor.h"

class ScreenClear :
	public GameProcessor
{
public:
	ScreenClear(const int32 &width, const int32 &height, const std::wstring &name);
	virtual ~ScreenClear() = default;

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void Render() override;
};

