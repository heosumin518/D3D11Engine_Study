#include "pch.h"
#include "AssimpTool.h"
#include "Converter.h"

void AssimpTool::Init()
{
	{
		shared_ptr<Converter> converter = make_shared<Converter>();

		// FBX -> Memory
		//converter->ReadAssetFile(L"Tank/Tank.fbx");
		converter->ReadAssetFile(L"Zelda/zelda.fbx");

		// Memory -> CustomData
		converter->ExportMaterialData(L"Zelda/Zelda");
		converter->ExportModelData(L"Zelda/Zelda");

		// CustomData (File) -> Memory
	}

	{
		shared_ptr<Converter> converter = make_shared<Converter>();

		// FBX -> Memory
		converter->ReadAssetFile(L"Tank/Tank.fbx");
		//converter->ReadAssetFile(L"Zelda/zelda.fbx");

		// Memory -> CustomData
		converter->ExportMaterialData(L"Tank/Tank");
		converter->ExportModelData(L"Tank/Tank");

		// CustomData (File) -> Memory
	}

	{
		shared_ptr<Converter> converter = make_shared<Converter>();

		// FBX -> Memory
		converter->ReadAssetFile(L"Tower/Tower.fbx");

		// Memory -> CustomData
		converter->ExportMaterialData(L"Tower/Tower");
		converter->ExportModelData(L"Tower/Tower");

		// CustomData (File) -> Memory
	}
}

void AssimpTool::Update()
{

}

void AssimpTool::Render()
{

}
