#pragma once
#include "ShaderManager.h"
#include "D3DXREAD.h"
#include "FollowCamera.h"



class D3DXDROW
{
private:
	//頂点の構造体
	struct MY_VERTEX
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR2 vTex;
	};
	//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mWVP;//ワールドから射影までの変換行列
	};
	//オリジナルメッシュ
	struct MY_MESH
	{
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
	};
public:
	D3DXDROW();
	~D3DXDROW();
	HRESULT InitShader();//シェーダ初期化
	HRESULT InitD3D();//
	void Render(std::unique_ptr<FollowCamera>& camera);//描画
private:
	ShaderManager shadermanager;
	//↓モデルの種類ごと(モデルの構造が全て同一ならアプリにひとつ）
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	D3DXREAD* m_d3dxread;
	//↓モデルごと
	MY_MESH m_Mesh;
	ID3D11SamplerState* m_pSampleLinear;//テクスチャーのサンプラー
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー
	int birthcnt;

};

