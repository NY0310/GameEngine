#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>
#include "FollowCamera.h"
#include "Device.h"
#include "ShaderManager.h"
#include "AStar/Math.h"
#include "UsingDirectSimpleMath.h"


#pragma comment(lib, "d3dcompiler.lib")

using namespace UsingDirectSimpleMath;
using namespace Microsoft::WRL;



class InkParticleRenderer
{
public:
	//コンスタントバッファー出力データ
	struct ConstantInkData
	{
		Vector3 positon;
		Vector3 scale;
		Vector3	rotation;
		Vector4 color;
	};
	HRESULT Initialize();//初期化関数を全て実行する
	void Render(const vector<ConstantInkData> inkData);//全パーティクルのワールド行列を指定して描画処理を呼ぶ
	InkParticleRenderer() = default;
	~InkParticleRenderer();
private:
	void RenderSprite(DirectX::SimpleMath::Matrix & WVP, DirectX::SimpleMath::Vector4& color);//描画処理

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
		DirectX::SimpleMath::Matrix wvp;
		Vector4 color;
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

