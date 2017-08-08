/// <summary>
/// シェーダー関連便利クラス(シングルトン)
/// </summary>
#pragma once
#include <windows.h>
#include <d3d11_1.h>
#include <d3d11.h>
#include <d3dx10.h>
#include <d3dx11.h>
#include <memory>
#include "Device.h"






#pragma comment(lib, "d3dcompiler.lib")

//マクロ
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}



//using namespace DirectX;
//using namespace Microsoft::WRL;
using namespace std;


class ShaderManager
{
public:

	//Devicesクラスのインスタンスを取得する
	static ShaderManager& Get() {
		if (shadermanager.get() == nullptr)
		{
			shadermanager.reset(new ShaderManager());
		}
		return *shadermanager.get();
	}

	HRESULT MakeShader(LPSTR, LPSTR, LPSTR, void**, ID3DBlob**);

	D3DXMATRIX MatrixToD3DXMATRIX(DirectX::SimpleMath::Matrix matrix);
	D3DXVECTOR3 VectorToD3DXVECTOR3(DirectX::SimpleMath::Vector3 vector3);
	DirectX::SimpleMath::Vector3 D3DXVECTOR3ToVector(D3DXVECTOR3 d3dxvector3);

	DirectX::SimpleMath::Matrix D3DXMATRIXToMatrix(D3DXMATRIX d3dxmatrix);
	ShaderManager() {}

private:
	static std::unique_ptr<ShaderManager> shadermanager;

};

