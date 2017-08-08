#include <d3d11_1.h>
#include <SimpleMath.h>
#include "FollowCamera.h"
#include <vector>
#include "Device.h"
#include "PARTICLE.h"
#include "ShaderManager.h"


#pragma comment(lib, "d3dcompiler.lib")

//頂点の構造体
struct SimpleVertex
{
	D3DXVECTOR3 Pos; //位置
					 //D3DXVECTOR2 Tex; //テクスチャー座標
};
//シェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
struct CONSTANT_BUFFER
{
	D3DXMATRIX mWVP;
};


#pragma once
class ToManageParticle
{
public:
	inline ToManageParticle() {}
	void Init();//初期化関数を全て実行する
	HRESULT InitD3D();//d3d初期化
	HRESULT InitModel();//モデル初期化
	void Update();//更新処理
	void Render(std::unique_ptr<FollowCamera>& camera);//全パーティクルのワールド行列を指定して描画処理を呼ぶ
	void RenderSprite(D3DXMATRIX & WVP);//描画処理

	void AddParticle(DirectX::SimpleMath::Vector3 enemypos);//パーティクル追加
	//Getter
	inline const std::vector<PARTICLE*>& GetParticle() { return m_pParticle; }
	//Setter
private:
	ID3D11Buffer* m_pConstantBuffer;
	ID3D11InputLayout* pCompiledShader2;

	ID3D11GeometryShader* m_pGeometryShader;//ジオメトリシェーダ
	ID3D11PixelShader* m_pPixelShader;//ピクセルシェーダ
	ID3D11VertexShader* m_pVertexShader;//バーテックスシェーダ
	ID3D11InputLayout* m_pVertexLayout;

	ID3D11SamplerState* m_pSampler;//テクスチャーのサンプラー
	ID3D11ShaderResourceView* m_pTexture;//テクスチャー（用意するのはリソースビューだけでいい）
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11BlendState* m_pBlendState;
	ID3D11DepthStencilState* m_DepthStencilState;




	std::vector< PARTICLE*> m_pParticle;//パーティクル
	std::vector<int> m_Particlecnt;

	int Particlecnt = 0;
	int MAX_PARTICLE = 100;


	ShaderManager shadermanager;//シェーダー関連
};

