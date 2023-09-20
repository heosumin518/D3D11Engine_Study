#pragma once

struct Vertex
{
	Vector3 position;		// 정점 위치 정보
	Color color;			// 정점 색상 정보

	Vertex() = default;		// 기본생성자.  없애야할수도..?

	Vertex(float x, float y, float z) : position(x, y, z) { }
	Vertex(Vector3 position) : position(position) { }

	Vertex(Vector3 position, Color color)
		: position(position), color(color) { }
};


struct TransformData
{
	Matrix world = Matrix::Identity;
	Matrix view = Matrix::Identity;
	Matrix projection = Matrix::Identity;
};