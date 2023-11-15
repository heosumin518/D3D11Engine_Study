#include "pch.h"
#include "AssimpTool.h"
#include "Converter.h"

void AssimpTool::Init()
{
	{
		shared_ptr<Converter> converter = make_shared<Converter>();

		// FBX -> Memory
		converter->ReadAssetFile(L"House/House.fbx");
		//converter->ReadAssetFile(L"Zelda/zelda.fbx");

		// Memory -> CustomData
		converter->ExportMaterialData(L"House/House");
		converter->ExportModelData(L"House/House");

		// CustomData (File) -> Memory

	}
}

void AssimpTool::Update()
{

}

void AssimpTool::Render()
{

}
