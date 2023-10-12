#pragma once
#include "GameProcessor.h"

struct Vertex
{
	Vector3 position;
	Color color;
};

class RenderTriangle :
	public GameProcessor
{
public:
	RenderTriangle(const int32& width, const int32& height, const std::wstring& name);
	virtual ~RenderTriangle() = default;

	void Initialize() override;
	void Update() override;
	void Render() override;

	void CreateGeometry() override;
	void CreateInputLayout() override;

private:
	vector<Vertex> m_vertices;
	vector<WORD> m_indices;
};

