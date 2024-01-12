#pragma once

#include <comdef.h>

// STL
#include <map>
#include <vector>
#include <string>
#include <memory>
#include <filesystem>
using namespace std;

// ImGUI
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>

// WIN
#include <windows.h>
#include <assert.h>

//Assimp
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

// DirectX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <directxtk/SimpleMath.h>
#include <DirectXTex.h>
#include <DirectXTex.inl>

#include <Directxtk/DDSTextureLoader.h>
#include <Directxtk/WICTextureLoader.h>

//#include <DirectXMath.h>
using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace Microsoft::WRL;

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")

// Helpers
#include "Define.h"
#include "Helper.h"

// Managers
#include "Game.h"
#include "Graphics.h"
#include "TimeManager.h"
#include "RenderManager.h"

// Engine
#include "IExecute.h"
#include "ConstantBuffer.h"

// Temp
#include "Model.h"
#include "ModelLoader.h"
