#pragma once
#include "ShaderManager.h"
#include "Device.h"
#include "FollowCamera.h"


#pragma once
class DisplacementMapping
{
private:
	//頂点の構造体
	struct SimpleVertex
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR2 vTex;
		D3DXVECTOR3 vNorm;
	};

	//コンスタントバッファー
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mWVP;//ワールドのプロジェクション行列
		D3DXMATRIX mW;//ワールド行列
		ALIGN16 D3DXVECTOR3 vEyePos;//各頂点から見た、視点の位置
		ALIGN16 float fMinDistance;
		ALIGN16 float fMaxDistance;
		ALIGN16 int iMaxDevide;
		ALIGN16 D3DXVECTOR4 LightDir;
		ALIGN16 D3DXVECTOR4 WaveMove;
	};

	//オリジナル　マテリアル構造体
	struct MY_MATERIAL
	{
		CHAR szName[111];
		D3DXVECTOR4 Ka;//アンビエント
		D3DXVECTOR4 Kd;//ディフューズ
		D3DXVECTOR4 Ks;//スペキュラー
		CHAR szTextureName[110];//テクスチャーファイル名
	};



	//オリジナルメッシュ
	struct MY_MESH
	{
		DWORD dwNumVert;
		DWORD dwNumFace;
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
	};


	ShaderManager& shadermanager = ShaderManager::Get();
	ID3D11Device* pDevice = Devices::Get().Device().Get();
	ID3D11DeviceContext* pDeviceContext = Devices::Get().Context().Get();
	//↓モデルの種類ごと(モデルの構造が全て同一ならアプリにひとつ）
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11HullShader* m_pHullShader;
	ID3D11DomainShader* m_pDomainShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	//↓モデルごと
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11SamplerState* m_pSamPoint;//テクスチャーのサンプラー
	ID3D11ShaderResourceView* m_pHeightTexture;//テクスチャー
	ID3D11ShaderResourceView* m_pDensityTexture;//テクスチャー
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー

	MY_MESH m_Mesh;
	MY_MATERIAL m_Material;

	D3DXVECTOR4 WaveMove;
	D3DXVECTOR4 WaveHeight;

public:
	DisplacementMapping();
	~DisplacementMapping();
	HRESULT InitD3D();
	HRESULT InitShader();
	HRESULT InitPolygon();
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH* pMesh);
	HRESULT LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL* pMaterial);

	void Render(std::unique_ptr<FollowCamera>& camera);

};

