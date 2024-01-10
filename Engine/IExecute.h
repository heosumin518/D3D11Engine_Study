#pragma once

namespace Engine
{
	class IExecute
	{
	public:
		virtual void Init() abstract;
		virtual void Update() abstract;
		virtual void Render() abstract;
	};
}
