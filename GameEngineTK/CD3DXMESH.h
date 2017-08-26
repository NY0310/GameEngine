#pragma once
#include "ShaderManager.h"
#include "FollowCamera.h"

#include<d3dx9.h>
#include "ShaderManager.h"
#include "Device.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

////Xファイル内のアニメーション階層を読み下すクラス。
//class MY_H_HIERARCHY : public ID3DXAllocateHierarchy
//{
//public:
//	MY_H_HIERARCHY() {}
//	STDMETHOD(CreateFrame)(THIS_ LPCSTR, LPD3DXFRAME *);
//	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*,
//	CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD *, LPD3DXSKININFO, LPD3DXMESHCONTAINER *);
//	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
//	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
//};
class CD3DXMESH
{
public:

	//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
	struct SIMPLECONSTANT_BUFFER0
	{
		D3DXMATRIX mW;//ワールド行列
		D3DXMATRIX mWVP;//ワールドから射影までの変換行列
		D3DXVECTOR4 vLightDir;//ライト方向
		D3DXVECTOR4 vEyes;//カメラ位置

	};
	struct SIMPLECONSTANT_BUFFER1
	{
		D3DXVECTOR4 vAmbient;//アンビエント光
		D3DXVECTOR4 vDiffuse;//ディヒューズ色
		D3DXVECTOR4 vSpecular;//鏡面反射
	};

	//オリジナル　マテリアル構造体
	struct MY_MATERIAL
	{
		CHAR szName[100];
		D3DXVECTOR4 Ambient;//アンビエント
		D3DXVECTOR4 Diffuse;//ディヒューズ
		D3DXVECTOR4 Specular;//スペキュラー
		CHAR szTextureName[256];//テクスチャファイル名
		ID3D11ShaderResourceView* pTexture;
		DWORD dwNumFace;//そのマテリアルであるポリゴン数
	};
	//頂点の構造体
	struct MY_VERTEX_TEX
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vNorm;
		D3DXVECTOR2 vTex;
	};

	////頂点の構造体
	//struct MY_VERTEX_NOTEX
	//{
	//	D3DXVECTOR3 vPos;
	//	D3DXVECTOR3 vNorm;
	//};



	////オリジナルメッシュ
	//struct MY_MESH
	//{
	//	ID3D11Buffer* pVertexBuffer;
	//	ID3D11Buffer* pIndexBuffer;
	//};



	////パーツメッシュ　アプリ定義メッシュ
	////(スタティックメッシュのサンプルにおけるCD3DXMESHクラスと同等)
	//struct PARTS_MESH
	//{
	//	DWORD dwNumMaterial;
	//	ID3D11Buffer* pVertexBuffer;
	//	ID3D11Buffer* ppIndexBuffer;
	//	char TextureFileName[8][256];//テクスチャファイル名(8名まで)
	//	MY_MATERIAL* pMaterial;
	//	bool Tex;
	//};
public:
	CD3DXMESH();
	~CD3DXMESH();
	HRESULT Init(LPSTR);
	HRESULT InitDx9();
	HRESULT LoadXMesh(LPSTR FileName);
	HRESULT InitShader();//シェーダ初期化

	void CD3DXMESH::Render(unique_ptr<FollowCamera>& camera, D3DXVECTOR3& vLight);
		//描画
private:
	//Date
	DWORD m_dwNumMaterial;
	LPD3DXMESH m_pMesh;
	ShaderManager shadermanager;
	D3DXVECTOR3 m_vPos;
	float m_fYaw, m_fPitch, m_fRoll;
	float m_fScale;
	//Dx9
	LPDIRECT3D9 m_pD3d9;
	LPDIRECT3DDEVICE9 m_pDevice9;
	D3DMATERIAL9* pMeshMaterials;
	DWORD m_NumAttr;
	DWORD m_AttrID[300];//300属性まで
	bool m_Tex;
	//LPDIRECT3DTEXTURE9* pMeshTextures;
	//↓モデルの種類ごと(モデルの構造が全て同一ならアプリにひとつ）
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pConstantBuffer2;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer** m_ppIndexBuffer;
	//↓モデルごと
	char m_TextureFileName[8][256];//テクスチャーファイル名（８枚まで）
	ID3D11SamplerState* m_pSampleLinear;//テクスチャーのサンプラー
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー
	MY_MATERIAL* m_pMaterial;
	bool m_Texture;//テクスチャデータが存在するか
	int birthcnt;//生成されてからのフレーム数



};

