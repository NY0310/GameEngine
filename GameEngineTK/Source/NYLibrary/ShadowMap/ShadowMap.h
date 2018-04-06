/// <summary>
/// シャドウマップ(シングルトン)
/// </summary>
#pragma once
#include <d3d11_1.h>
#include <wrl/client.h>
#include <memory>

#include "../ShaderManager/ShaderManager.h"
#include "../Device/Device.h"



using namespace Microsoft::WRL;

namespace NYLibrary
{
	class ShadowMap
	{
	public:
		struct ConstantBuffer
		{
			D3DXMATRIX wlp;//ワールドライトプロジェクション行列
		};

		//デストラクタ
		~ShadowMap() = default;
		//コピーコンストラクタ禁止
		ShadowMap(const ShadowMap&) = delete;
		//代入禁止
		ShadowMap& operator=(const ShadowMap&) = delete;
		//シャドウマップのインスタンスを取得する
		static ShadowMap* GetInstance();
		//初期化
		void Initilize();
		//終了
		void Finalize();
		//レンダーターゲットビューとデプスステンシルビューをクリア
		void ClearRTVAndDSV();
		//レンダーターゲットビューを設定
		void OMSetRenderTargets();
		//シャドウマップのシェーダーを設定する
		void SetShader();
		//深度ステンシルビューを取得
		const ComPtr<ID3D11DepthStencilView>& GetDepthStencilView() { return depthStencilView; }
		//レンダーターゲットビューを取得する
		const ComPtr<ID3D11RenderTargetView>& GetRenderTargetView() { return  renderTargetView; }
		//シェーダーリソースビューを取得
		const ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() { return shaderResourceView; }
		//コンスタントバッファー取得
		const ComPtr<ID3D11Buffer>& GetConstantBuffer() { return constantBuffer; }
		//サンプラー取得
		const ComPtr<ID3D11SamplerState>& GetSamplerState() { return sampler; }
	private:
		//コンストラクタ
		ShadowMap() = default;
		//デプスステンシル用のテクスチャ作成
		void CreateDSVTex2D();
		//深度マップ用テクスチャ
		void CreateRTVTex2D();
		//深度ステンシルビュー作成
		void CreateDepthStencilView();
		//レンダーターゲットビュー作成
		void CreateRenderTargetView();
		//シェーダーリソースビューを作成
		void CreateShaderResourceView();
		//シェーダー作成
		void CreateShader();
		//コンスタントバッファー作成
		void CreateConstantBuffer();
		//サンプラー作成
		void SamplerCreate();
		//自クラスインスタンス
		static std::unique_ptr<ShadowMap> shadowMap;
		//デプスステンシルビュー用のテクスチャ
		ComPtr<ID3D11Texture2D> DSVTex2D;
		//深度マップ用テクスチャ
		ComPtr<ID3D11Texture2D> RTVTex2D;
		//深度デプスステンシルビュー
		ComPtr<ID3D11DepthStencilView> depthStencilView;
		//レンダリングターゲットビュー
		ComPtr<ID3D11RenderTargetView> renderTargetView;
		//シェーダーリソースビュー
		ComPtr<ID3D11ShaderResourceView> shaderResourceView;
		//バーテックスシェーダー
		ComPtr<ID3D11VertexShader> vertexShader;
		//ピクセルシェーダ
		ComPtr<ID3D11PixelShader> pixelShader;
		//コンスタントバッファー
		ComPtr<ID3D11Buffer> constantBuffer;
		//サンプラー
		ComPtr<ID3D11SamplerState> sampler;
	};
}