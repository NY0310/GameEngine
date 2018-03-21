/// <summary>
/// シェーダー関連便利クラス(シングルトン)
/// </summary>
#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <memory>
#include "../Device/Device.h"






#pragma comment(lib, "d3dcompiler.lib")

//マクロ
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}
#define ALIGN16 _declspec(align(16))




namespace NYLibrary
{
	HRESULT MakeShader(LPSTR, LPSTR, LPSTR, void**, ID3DBlob**);
	//bool SameVSShader()
};
