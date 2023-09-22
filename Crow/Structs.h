#pragma once

struct Vertex
{
	Vector3 position;		// 정점 위치 정보
	Vector3 normal;
	//Color color;			// 정점 색상 정보 라이팅 하면 이거 대신 텍스처 사용할것.
	// Vector2 Tex;			// 라이팅 다 하면 이거 쓰기

	Vertex() = default;		// 기본생성자.  없애야할수도..?

	Vertex(float x, float y, float z) : position(x, y, z) { }
	Vertex(Vector3 position) : position(position) { }

	//Vertex(Vector3 position, Color color)
	//	: position(position), color(color) { }

	Vertex(Vector3 position, Vector3 normal)
		: position(position), normal(normal) { }

	//Vertex(Vector3 position, Vector3 normal, Color color)
	//	: position(position), normal(normal), color(color) { }
};


struct TransformData
{
	Matrix world = Matrix::Identity;
	Matrix view = Matrix::Identity;
	Matrix projection = Matrix::Identity;

	Vector4 lightDir[2];
	Vector4 lightColor[2];
	Vector4 outputColor;
};