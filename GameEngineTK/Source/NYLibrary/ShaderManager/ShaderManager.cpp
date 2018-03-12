#include "ShaderManager.h"


//#pragma comment(lib,"d3dx10.lib")
//#pragma comment(lib,"d3dx11.lib")

// シングルトンのためのポインタ変数を初期化する
//std::unique_ptr<ShaderManager> ShaderManager::shadermanager(nullptr);

using Microsoft::WRL::ComPtr;
using namespace NYLibrary;


HRESULT NYLibrary::MakeShader(LPSTR szFileName,LPSTR szFuncName,LPSTR szProfileName,void** ppShader,ID3DBlob** ppBlob)
{

	//hlslファイルを読み込みシェーダーを作成する

		auto& devices = Devices::Get();


		ID3DBlob *pErrors = NULL;
		HRESULT res = D3DX11CompileFromFileA(szFileName, NULL, NULL, szFuncName, szProfileName, D3D10_SHADER_DEBUG | D3D10_SHADER_SKIP_OPTIMIZATION, 0, NULL, ppBlob, &pErrors, NULL);
		if (FAILED(res))
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


