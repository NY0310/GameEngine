/// <summary>
/// OBJファイルの読み込み描画
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include "ShaderManager.h"
#include "Device.h"
#include "FollowCamera.h"



class OBJ
{
public:
	//頂点の構造体
	struct SimpleVertex
	{
		D3DXVECTOR3 Pos; //位置
		D3DXVECTOR3 Normal;//法線
		D3DXVECTOR2 Tex; //テクスチャー座標
	};

	//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		//D3DXMATRIX mW;//ワールド行列
		D3DXMATRIX mWVP;//ワールドから射影までの変換行列
		//D3DXVECTOR4 vLightDir;//ライト方向
		//D3DXVECTOR4 vColor;//ディヒューズ色
		//D3DXVECTOR4 vSpecular;//鏡面反射光
		//D3DXVECTOR4 vEyes;//カメラ位置
	};


	//オリジナルメッシュ
	struct MY_MESH
	{
		DWORD dwNumVert;
		DWORD dwNumFace;
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
	};
	OBJ();
	~OBJ();
	void Init();
	HRESULT InitD3D();
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);
	void Render(std::unique_ptr<FollowCamera>& camera);
private:
	ID3D11PixelShader* m_pPixelShader;
	ID3D11VertexShader* m_pVertexShader;

	ID3D11Buffer* m_pConstantBuffer;
	MY_MESH m_Mesh;
	ID3D11InputLayout* m_pVertexLayout;

	ShaderManager shadermanager;//シェーダー関連


	D3DXVECTOR3 m_vLight;

	ID3D11SamplerState* m_pSampleLimear;//テクスチャサンプラ
	ID3D11ShaderResourceView* m_pTexture;//テクスチャ
	int birthcnt;

};

