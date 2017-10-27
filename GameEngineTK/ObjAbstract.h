//#pragma once
//#include "Device.h"
//#include "ShaderManager.h"
//
//class ObjAbstract
//{
//protected:
//
//
//
//
//	//オリジナルメッシュ
//	struct MY_MESH
//	{
//		DWORD dwNumVert;
//		DWORD dwNumFace;
//		ID3D11Buffer* pVertexBuffer;
//		ID3D11Buffer* pIndexBuffer;
//		D3DXMATRIX mWorld;
//	};
//
//
//	//オリジナル　マテリアル構造体
//	struct MY_MATERIAL
//	{
//		CHAR szName[111];
//		D3DXVECTOR4 Ka;//アンビエント
//		D3DXVECTOR4 Kd;//ディヒューズ
//		D3DXVECTOR4 Ks;//スペキュラー
//		CHAR szTextureName[110];//テクスチャーファイル名
//	};
//public:
//	ObjAbstract();
//	~ObjAbstract();
//
//protected:
//	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);
//	HRESULT LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL * pMarial);
//
//	ID3D11Device* pDevice = Devices::Get().Device().Get();
//	ID3D11DeviceContext* pDeviceContext = Devices::Get().Context().Get();
//
//	MY_MESH m_Mesh;
//	MY_MATERIAL m_Material;//マテリアル構造体
//	ID3D11SamplerState* m_pSampleLimear;//テクスチャサンプラ
//	ID3D11ShaderResourceView* m_pTexture;//テクスチャ
//
//
//};
//
