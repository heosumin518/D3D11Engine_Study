#pragma once
#include "pch.h"

class Model;

class ResourceManager
{
	DECLARE_SINGLE(ResourceManager);	// 씬 에서만 쓰일 거면 굳이 싱글턴으로 하지 않아도 될 듯 한데..

public:
	void Init();

	shared_ptr<Model> LoadModel(string path);

private:
	// StaticMesh Map
	// SkeletalMesh Map
	// Animation Map
	// Material
	// Model Map

	// Skeleton	// bone 정보

};

