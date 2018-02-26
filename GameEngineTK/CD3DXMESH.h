#pragma once
#include<d3dx9.h>
#include <vector>

#include "FollowCamera.h"
#include "ShaderManager.h"
#include "ShaderManager.h"
#include "Device.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

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

	//メッシュコンテナー構造体 フレーム内で使用
	/// <summary>
	/// Name
	/// メッシュ名。
	/// MeshData
	/// メッシュ内のデータのタイプ。
	/// pMaterials
	/// メッシュ材料の配列。
	/// pEffects
	/// デフォルトエフェクトパラメータのセットへのポインタ。
	/// NumMaterials
	/// メッシュ内のマテリアルの数。
	/// pAdjacency
	/// 隣接情報を含むメッシュの三角形あたり3つのDWORDの配列へのポインタ。
	/// pSkinInfo
	/// スキン情報インターフェイスへのポインタ。
	/// pNextMeshContainer
	/// 次のメッシュコンテナへのポインタ。
	/// </summary>
	struct MYHMESHCONTAINER : public D3DXMESHCONTAINER
	{
		//アプリケーションからテクスチャー リソースを操作
		LPDIRECT3DTEXTURE9*  ppTextures;
	};

	//パーツメッシュ　アプリ定義メッシュ
	//(スタティックメッシュのサンプルにおけるCD3DXMESHクラスと同等)
	struct PARTS_MESH
	{
		DWORD dwNumMaterial;
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer** ppIndexBuffer;
		char TextureFileName[8][256];//テクスチャファイル名(8名まで)
		MY_MATERIAL* pMaterial;
		bool Tex;
	};

	//派生フレーム構造体
	struct MYHFRAME : public D3DXFRAME
	{
		D3DXMATRIXA16 CombinedTransformationMatrix;
		PARTS_MESH* pPartsMesh;
		MYHFRAME()
		{
			ZeroMemory(this, sizeof(MYHFRAME));
		}
	};

//Xファイル内のアニメーション階層を読み下すクラス。
/// <summary>
/// ID3DXAllocateHierarchy
/// CreateFrame	フレーム オブジェクトの割り当てを要求する。
/// CreateMeshContainer	メッシュ コンテナ オブジェクトの割り当てを要求する。
///	DestroyFrame	フレーム オブジェクトの割り当て解除を要求する。
///DestroyMeshContainer	メッシュ コンテナ オブジェクトの割り当て解除を要求する。
/// </summary>
class MY_H_HIERARCHY : public ID3DXAllocateHierarchy
{
public:
	MY_H_HIERARCHY() {}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR, LPD3DXFRAME *);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*,
	CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD *, LPD3DXSKININFO, LPD3DXMESHCONTAINER *);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
};


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


	//頂点の構造体
	struct MY_VERTEX_TEX
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vNorm;
		D3DXVECTOR2 vTex;
	};

	//頂点の構造体
	struct MY_VERTEX_NOTEX
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR3 vNorm;
	};



	//オリジナルメッシュ
	struct MY_MESH
	{
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
	};



public:
	CD3DXMESH();
	~CD3DXMESH();
	HRESULT Init(LPSTR);
	HRESULT InitDx9();
	HRESULT InitShader();//シェーダ初期化

	void Render(unique_ptr<FollowCamera>& camera, D3DXVECTOR3& vLight);
		//描画
	HRESULT LoadXAnimMesh(LPSTR FileName);
	void BuildAllMesh(D3DXFRAME* p);
	HRESULT CreateAppMeshFromD3DXMesh(LPD3DXFRAME p);
	void DrawFrame(LPD3DXFRAME p, unique_ptr<FollowCamera>& camera, D3DXVECTOR3& vLight);
	void DrawPartsMesh(PARTS_MESH* p, D3DXMATRIX World, unique_ptr<FollowCamera>& camera, D3DXVECTOR3& vLight);
	void UpdateHierarchyMatrices(D3DXFRAME* p, LPD3DXMATRIX pParentMatrix);
	inline LPD3DXANIMATIONCONTROLLER GetAnimController() {
		return m_pAnimController;
	}
	float&  GetfYaw() { return m_fYaw; }


	void ChangeAnimSet(int index);

private:
	//Date
	//DWORD m_dwNumMaterial;
	LPD3DXMESH m_pMesh;
	ShaderManager shadermanager;
	D3DXVECTOR3 m_vPos;
	float m_fYaw, m_fPitch, m_fRoll;
	float m_fScale;
	//アニメ関連
	MY_H_HIERARCHY* m_pHierarchy;
	/// <summary>
	/// フレームの名前。
		/*TransformationMatrix
		トランスフォーム行列。
		pMeshContainer
		メッシュ コンテナへのポインタ。
		pFrameSibling
		兄弟フレームへのポインタ。
		pFrameFirstChild
		子フレームへのポインタ。*/
	/// </summary>
	LPD3DXFRAME m_pFrameRoot;
	LPD3DXANIMATIONCONTROLLER m_pAnimController;
	//Dx9
	LPDIRECT3D9 m_pD3d9;
	LPDIRECT3DDEVICE9 m_pDevice9;
	D3DMATERIAL9* pMeshMaterials;
	//DWORD m_NumAttr;
	DWORD m_AttrID[300];//300属性まで
	bool m_Tex;
	//LPDIRECT3DTEXTURE9* pMeshTextures;
	//↓モデルの種類ごと(モデルの構造が全て同一ならアプリにひとつ）
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11InputLayout* m_pVertexLayout_NoTex;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11VertexShader* m_pVertexShaderNoTex;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11PixelShader* m_pPixelShaderNoTex;
	ID3D11Buffer* m_pConstantBuffer0;
	ID3D11Buffer* m_pConstantBuffer1;
	ID3D11Buffer* m_pVertexBuffer0;
	ID3D11Buffer** m_ppIndexBuffer;
	//↓モデルごと
	//char m_TextureFileName[8][256];//テクスチャーファイル名（８枚まで）
	ID3D11SamplerState* m_pSampleLinear;//テクスチャーのサンプラー
	//ID3D11ShaderResourceView* m_pTexture;//テクスチャー
	//MY_MATERIAL* m_pMaterial;
	//bool m_Texture;//テクスチャデータが存在するか
	int birthcnt;//生成されてからのフレーム数

	vector<LPD3DXANIMATIONSET> m_pAnimSet;//アニメーションセットに対応


};

