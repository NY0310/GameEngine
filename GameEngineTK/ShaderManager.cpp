#include "ShaderManager.h"


//#pragma comment(lib,"d3dx10.lib")
//#pragma comment(lib,"d3dx11.lib")

// シングルトンのためのポインタ変数を初期化する
std::unique_ptr<ShaderManager> ShaderManager::shadermanager(nullptr);

using namespace DirectX;
using namespace DirectX::SimpleMath;
using Microsoft::WRL::ComPtr;


HRESULT ShaderManager::MakeShader(LPSTR szFileName,LPSTR szFuncName,LPSTR szProfileName,void** ppShader,ID3DBlob** ppBlob)
{

	//hlslファイルを読み込みシェーダーを作成する

		auto& devices = Devices::Get();


		ID3DBlob *pErrors = NULL;
		if (FAILED(D3DX11CompileFromFileA(szFileName, NULL, NULL, szFuncName, szProfileName, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, ppBlob, &pErrors, NULL)))
		{
			char*p = (char*)pErrors->GetBufferPointer();
			MessageBoxA(0, p, 0, MB_OK);
			return E_FAIL;
		}
		SAFE_RELEASE(pErrors);
		char szProfile[3] = { 0 };
		memcpy(szProfile, szProfileName, 2);
		if (strcmp(szProfile, "vs") == 0)// Shader
		{
			if (FAILED(devices.Device().Get()->CreateVertexShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11VertexShader**)ppShader))) return E_FAIL;
		}
		if (strcmp(szProfile, "ps") == 0)//Pixel Shader
		{
			if (FAILED(devices.Device().Get()->CreatePixelShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11PixelShader**)ppShader))) return E_FAIL;
		}
		if (strcmp(szProfile, "gs") == 0)//Geometry Shader
		{
			if (FAILED(devices.Device().Get()->CreateGeometryShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11GeometryShader**)ppShader))) return E_FAIL;
		}
		if (strcmp(szProfile, "hs") == 0)//Hull Shader
		{
			if (FAILED(devices.Device().Get()->CreateHullShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11HullShader**)ppShader))) return E_FAIL;
		}
		if (strcmp(szProfile, "ds") == 0)//Domain Shader
		{
			if (FAILED(devices.Device().Get()->CreateDomainShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11DomainShader**)ppShader))) return E_FAIL;
		}
		if (strcmp(szProfile, "cs") == 0)//Compute Shader
		{
			if (FAILED(devices.Device().Get()->CreateComputeShader((*ppBlob)->GetBufferPointer(), (*ppBlob)->GetBufferSize(), NULL, (ID3D11ComputeShader**)ppShader))) return E_FAIL;
		}


		return S_OK;
}


D3DXVECTOR4 ShaderManager::VectorToD3DXVECTOR4(DirectX::SimpleMath::Vector4 vector4)
{
	float x = vector4.x;
	float y = vector4.y;
	float z = vector4.z;
	float w = vector4.w;

	D3DXVECTOR4 d3dxvector4 = D3DXVECTOR4(x, y, z,w);
	return d3dxvector4;

}




D3DXVECTOR3 ShaderManager::VectorToD3DXVECTOR3(DirectX::SimpleMath::Vector3 vector3)
{
	float x = vector3.x;
	float y = vector3.y;
	float z = vector3.z;
	D3DXVECTOR3 d3dxvector3 = D3DXVECTOR3(x, y, z);
	return d3dxvector3;

}


D3DXVECTOR2 ShaderManager::VectorToD3DXVECTOR2(DirectX::SimpleMath::Vector2 vector2)
{
	float x = vector2.x;
	float y = vector2.y;

	D3DXVECTOR2 d3dxvector2 = D3DXVECTOR2(x, y);
	return d3dxvector2;

}

Vector4 ShaderManager::D3DXVECTOR4ToVector(D3DXVECTOR4 d3dxvector4)
{
	float x = d3dxvector4.x;
	float y = d3dxvector4.y;
	float z = d3dxvector4.z;
	float w = d3dxvector4.w;

	Vector4 vector4 = Vector4(x, y, z,w);
	return vector4;
}


Vector3 ShaderManager::D3DXVECTOR3ToVector(D3DXVECTOR3 d3dxvector3)
{
	float x = d3dxvector3.x;
	float y = d3dxvector3.y;
	float z = d3dxvector3.z;
	Vector3 vector3 = Vector3(x, y, z);
	return vector3;
}


Vector2 ShaderManager::D3DXVECTOR2ToVector(D3DXVECTOR2 d3dxvector2)
{
	float x = d3dxvector2.x;
	float y = d3dxvector2.y;
	Vector2 vector2 = Vector2(x, y);
	return vector2;
}


Matrix ShaderManager::D3DXMATRIXToMatrix(D3DXMATRIX d3dxmatrix)
{
	const int MATRIX_NUM_QUARTER = 4;
	Matrix matrix;
	for (int i = 0; i < MATRIX_NUM_QUARTER; i++)
	{
		for (int j = 0; j < MATRIX_NUM_QUARTER; j++)
		{
			matrix.m[i][j] = d3dxmatrix.m[i][j];
		}
	}


	return matrix;
}

D3DXMATRIX ShaderManager::MatrixToD3DXMATRIX(DirectX::SimpleMath::Matrix matrix)
{
	const int MATRIX_NUM_QUARTER = 4;
	D3DXMATRIX d3dxmatrix;
	for (int i = 0; i < MATRIX_NUM_QUARTER; i++)
	{
		for (int j = 0; j < MATRIX_NUM_QUARTER; j++)
		{
			d3dxmatrix.m[i][j] = matrix.m[i][j];
		}
	}

	return d3dxmatrix;
}
