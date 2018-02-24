#pragma once
#include <memory>
#include "Device.h"
#include "ShaderManager.h"
#include "FollowCamera.h"
#include "AStar/Math.h"
#include "MatrixObject.h"
#include "ShaderManager.h"


using namespace Microsoft::WRL;

#define MAX_BONES 255

//マクロ
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }


//シェーダーに渡す値
struct LightAndEyeGlobal
{
	D3DXVECTOR4 vLightDir;//ライト方向
	D3DXVECTOR4 vEye;//カメラ位置
};

struct SkinMeshGlobal
{
	D3DXMATRIX mW;//ワールド行列
	D3DXMATRIX mWVP;//ワールドから射影までの変換行列
	D3DXVECTOR4 vAmbient;//アンビエント光
	D3DXVECTOR4 vDiffuse;//ディフューズ色
	D3DXVECTOR4 vSpecular;//鏡面反射
};

struct BoneGlobal
{
	D3DXMATRIX boneMatrix[MAX_BONES];
	BoneGlobal()
	{
		for (int i = 0; i<MAX_BONES; i++)
		{
			D3DXMatrixIdentity(&boneMatrix[i]);
		}
	}
};

//オリジナル　マテリアル構造体
struct SkinMaterial
{
	CHAR szName[110];
	D3DXVECTOR4 Ka;//環境光
	D3DXVECTOR4 Kd;//拡散反射光
	D3DXVECTOR4 Ks;//鏡面反射光
	CHAR textureName[512];//テクスチャーファイル名
	ComPtr<ID3D11ShaderResourceView> texture;
	DWORD numFace;//そのマテリアルであるポリゴン数
	SkinMaterial()
	{
		ZeroMemory(this, sizeof(SkinMaterial));
	}
	~SkinMaterial()
	{
		texture.Reset();
	}
};

//頂点構造体
struct SkinVertex
{
	D3DXVECTOR3 pos;//頂点位置
	D3DXVECTOR3 norm;//頂点法線
	D3DXVECTOR2 uv;//UV座標
	UINT boneIndex[4];//ボーン　番号
	float boneWeight[4];//ボーン　重み
	SkinVertex()
	{
		ZeroMemory(this, sizeof(SkinVertex));
	}
};
//ボーン構造体
struct Bone
{
	D3DXMATRIX bindPose;//初期ポーズ（ずっと変わらない）
	D3DXMATRIX newPose;//現在のポーズ（その都度変わる）
	DWORD numChild;//子の数
	int childIndex[50];//自分の子の”インデックス" 50個まで
	CHAR name[256];

	Bone()
	{
		ZeroMemory(this, sizeof(Bone));
		D3DXMatrixIdentity(&bindPose);
		D3DXMatrixIdentity(&newPose);
	}
};


//派生フレーム構造体 (それぞれのメッシュ用の最終ワールド行列を追加する）
struct DerivedFrame : public D3DXFRAME
{
	D3DXMATRIX CombinedTransformationMatrix;
};


//派生メッシュコンテナー構造体(
//コンテナーがテクスチャを複数持てるようにポインターのポインターを追加する）
struct MYMESHCONTAINER : public D3DXMESHCONTAINER
{
	LPDIRECT3DTEXTURE9*  ppTextures;
	DWORD weight;//重みの個数（重みとは頂点への影響。）
	DWORD boneNum;//ボーンの数
	LPD3DXBUFFER boneBuffer;//ボーン・テーブル
	D3DXMATRIX** ppBoneMatrix;//全てのボーンのワールド行列の先頭ポインター
	D3DXMATRIX* pBoneOffsetMatrices;//フレームとしてのボーンのワールド行列のポインター	
};


//Xファイル内のアニメーション階層を読み下してくれるクラスを派生させる。
//ID3DXAllocateHierarchyは派生すること想定して設計されている。
class MY_HIERARCHY : public ID3DXAllocateHierarchy
{
public:
	MY_HIERARCHY() {}
	STDMETHOD(CreateFrame)(THIS_ LPCSTR, LPD3DXFRAME *);
	STDMETHOD(CreateMeshContainer)(THIS_ LPCSTR, CONST D3DXMESHDATA*, CONST D3DXMATERIAL*,
	CONST D3DXEFFECTINSTANCE*, DWORD, CONST DWORD *, LPD3DXSKININFO, LPD3DXMESHCONTAINER *);
	STDMETHOD(DestroyFrame)(THIS_ LPD3DXFRAME);
	STDMETHOD(DestroyMeshContainer)(THIS_ LPD3DXMESHCONTAINER);
};

//
//
//
class D3DXPARSER
{
public:

	MY_HIERARCHY cHierarchy;
	MY_HIERARCHY* m_pHierarchy;
	LPD3DXFRAME m_pFrameRoot;
	MYMESHCONTAINER* m_pContainer;
	LPD3DXANIMATIONCONTROLLER m_pAnimController;
	HRESULT LoadMeshFromX(LPDIRECT3DDEVICE9, LPSTR);
	HRESULT AllocateBoneMatrix(LPD3DXMESHCONTAINER);
	HRESULT AllocateAllBoneMatrices(LPD3DXFRAME);
	void UpdateFrameMatrices(LPD3DXFRAME, LPD3DXMATRIX);
	//
	int GetNumVertices();
	int GetNumFaces();
	int GetNumMaterials();
	int GetNumUVs();
	int GetNumBones();
	int GetNumBoneVertices(int iBoneIndex);
	DWORD GetBoneVerticesIndices(int iBoneIndex, int iIndexInGroup);
	double GetBoneVerticesWeights(int iBoneIndex, int iIndexInGroup);
	D3DXVECTOR3 GetVertexCoord(DWORD iIndex);
	D3DXVECTOR3 GetNormal(DWORD iIndex);
	D3DXVECTOR2 GetUV(DWORD iIndex);
	int GetIndex(DWORD iIndex);
	D3DXVECTOR4 GetAmbient(int iIndex);
	D3DXVECTOR4 GetDiffuse(int iIndex);
	D3DXVECTOR4 GetSpecular(int iIndex);
	CHAR* GetTexturePath(int index);
	float GetSpecularPower(int iIndex);
	int GetFaceMaterialIndex(int iFaceIndex);
	int GetFaceVertexIndex(int iFaceIndex, int iIndexInFace);
	D3DXMATRIX GetBindPose(int iBoneIndex);
	D3DXMATRIX GetNewPose(int iBoneIndex);
	CHAR* GetBoneName(int iBoneIndex);
};

//
//class SkinMesh
//SkinMesh オリジナルメッシュクラス
class SkinMesh
{
public:
	//Dx11
	ComPtr<ID3D11SamplerState> sampleLinear;
	ComPtr<ID3D11VertexShader> vertexShader;
	ComPtr<ID3D11PixelShader> pixelShader;
	ComPtr<ID3D11InputLayout> vertexLayout;
	ComPtr<ID3D11Buffer> lightAndEyeBuffer;
	ComPtr<ID3D11Buffer> skinMeshBuffer;
	ComPtr<ID3D11Buffer> boneBuffer;

	//メッシュ
	D3DXPARSER* d3dxMesh;
	DWORD numVert;
	DWORD numFace;
	DWORD numUV;
	ComPtr<ID3D11Buffer> vertexBuffer;
	ComPtr<ID3D11Buffer>* pIndexBuffer;
	SkinMaterial* material;
	std::vector<Triangle> triangles;

	DWORD numMaterial;

	std::unique_ptr<MatrixObject> matrixObject;

	//ボーン
	int numBone;
	Bone* boneArray;

	//メソッド
	SkinMesh();
	~SkinMesh();
	virtual HRESULT Initialize();
	HRESULT CreateIndexBuffer(DWORD dwSize, int* pIndex, ID3D11Buffer** ppIndexBuffer);
	virtual void Render();
	HRESULT CreateFromX(CHAR* szFileName);
	HRESULT ReadSkinInfo(SkinVertex*);
	void SetNewPoseMatrices(int frame);
	D3DXMATRIX GetBindPoseMatrix(int index);
	D3DXMATRIX GetCurrentPoseMatrix(int index);
	CHAR* GetBoneNames(int iBoneIndex);
	void UpDateTriangles();

};