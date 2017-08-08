/// <summary>
/// �V�F�[�_�[�֘A�֗��N���X(�V���O���g��)
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

//�}�N��
#define SAFE_RELEASE(x) if(x){x->Release(); x=NULL;}



//using namespace DirectX;
//using namespace Microsoft::WRL;
using namespace std;


class ShaderManager
{
public:

	//Devices�N���X�̃C���X�^���X���擾����
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

