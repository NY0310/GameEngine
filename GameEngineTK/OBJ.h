#pragma once
#include "ShaderManager.h"
#include "Device.h"
#include "FollowCamera.h"

//頂点の構造体
struct OBJSimpleVertex
{
	D3DXVECTOR3 Pos; //位置
					 //D3DXVECTOR2 Tex; //テクスチャー座標
};

//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
struct SIMPLESHADER_CONSTANT_BUFFER
{
	D3DXMATRIX mWVP;//ワールドから射影までの変換行列
};


//オリジナルメッシュ
struct MY_MESH
{
	DWORD dwNumVert;
	DWORD dwNumFace;
	ID3D11Buffer* pVertexBuffer;
	ID3D11Buffer* pIndexBuffer;
};


class OBJ
{
public:
	OBJ();
	~OBJ();
	void Init();
	HRESULT InitD3D();
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);
	void Render(std::unique_ptr<FollowCamera>& camera);
private:
	ID3D11PixelShader* m_Obj_pPixelShader;
	ID3D11VertexShader* m_Obj_pVertexShader;

	ID3D11Buffer* m_OBJpConstantBuffer;
	MY_MESH m_Mesh;
	ID3D11InputLayout* m_pVertexLayout;

	ShaderManager shadermanager;//シェーダー関連

};

