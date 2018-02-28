#pragma once
#include <vector>
#include "FollowCamera.h"
#include "Device.h"
#include "ShaderManager.h"
#include "AStar/Math.h"


#pragma comment(lib, "d3dcompiler.lib")

using namespace Microsoft::WRL;



class InkParticleRenderer
{
public:
	//コンスタントバッファー出力データ
	struct ConstantInkData
	{
		D3DXVECTOR3 positon;
		D3DXVECTOR3 scale;
		D3DXVECTOR3	rotation;
		D3DXVECTOR4 color;
	};
	HRESULT Initialize();//初期化関数を全て実行する
	void Render(const vector<ConstantInkData> inkData);//全パーティクルのワールド行列を指定して描画処理を呼ぶ
	InkParticleRenderer() = default;
	~InkParticleRenderer();
private:
	void RenderSprite(D3DXMATRIX & WVP, D3DXVECTOR4& color);//描画処理

private:
	//頂点の構造体
	struct vertexData
	{
		D3DXVECTOR3 position; //位置
		D3DXVECTOR2 uv; //テクスチャー座標
	};
	//シェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
	struct CONSTANT_BUFFER
	{
		D3DXMATRIX wvp;
		D3DXVECTOR4 color;
	};

	ComPtr<ID3D11VertexShader> vertexShader;//バーテックスシェーダ
	ComPtr<ID3D11GeometryShader> geometryShader;//ジオメトリシェーダ
	ComPtr<ID3D11PixelShader> pixelShader;//ピクセルシェーダ

	ComPtr<ID3D11Buffer> constantBuffer;

	ComPtr<ID3D11Buffer> vertexBuffer;//バーテックスバッファー
	ComPtr<ID3D11InputLayout> vertexLayout;//頂点インップットレイアウト

	ComPtr<ID3D11ShaderResourceView> texture;//テクスチャー
	ComPtr<ID3D11SamplerState> sampler;//テクスチャーのサンプラー
	ComPtr<ID3D11BlendState> blendState;//
	ComPtr<ID3D11DepthStencilState> depthStencilState;

	FollowCamera* camera = FollowCamera::GetInstance();

};

