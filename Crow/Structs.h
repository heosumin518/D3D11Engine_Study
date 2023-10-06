#pragma once

struct Vertex
{
	Vector3 position;		// 정점 위치 정보
	Vector2 uv;	
	Vector3 normal;

	//Vertex() = default;		// 기본생성자.  없애야할수도..?

	//Vertex(float x, float y, float z) : position(x, y, z) { }
	//Vertex(Vector3 position) : position(position) { }

	////Vertex(Vector3 position, Color color)
	////	: position(position), color(color) { }

	//Vertex(Vector3 position, Vector3 normal)
	//	: position(position), normal(normal) { }

	////Vertex(Vector3 position, Vector3 normal, Color color)
	////	: position(position), normal(normal), color(color) { }
};

// --------------------------------------------------------------------------------------------------------------------------------------------
// Constant Buffer
// --------------------------------------------------------------------------------------------------------------------------------------------
struct TransformData
{
	Matrix world;
	Matrix view;
	Matrix projection;
	//Matrix world = Matrix::Identity;
	//Matrix view = Matrix::Identity;
	//Matrix projection = Matrix::Identity;
	Vector4 lightDir;
	Vector4 lightColor;
};
