#pragma once
#include "IExecute.h"

namespace Client
{
	class DemoApp : public Engine::IExecute
	{
	public:
		void Init() override;
		void Update() override;
		void Render() override;
	};

}

