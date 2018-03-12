/// <summary>
/// 基本的なテクスチャ
/// 作成方法を見て用途に合っていなければ自作してください
/// </summary>
#pragma once
#include <d3d11_1.h>
#include <D3DX10.h>
#include <memory>
#include "../Device/Device.h"


namespace NYLibrary
{
	class SimpleTextures
	{
	public:
		//コンストラクタ
		SimpleTextures(D3DXVECTOR2 viewSize);
		SimpleTextures() = delete;
		//デストラクタ
		~SimpleTextures() = default;
		//初期化
		void Initialize();
		//終了
		void Finalize();
		//2Dテクスチャを取得
		Microsoft::WRL::ComPtr<ID3D11Texture2D>& GetTexture2D() { return texture2D; }
		//シェーダーリソースビューを取得
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() { return shaderResourceView; }
		//レンダリングターゲットビューを取得
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>& GetRenderTargetView() { return renderTargetView; }
		//深度ステンシルビューを取得
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>& GetDepthStencilView() { return depthStencilView; }
		//レンダリングターゲットを使用する
		void SetRenderTargets();
		//レンダリングターゲットをクリアする
		void ClearRenderTargetView();
	private:
		//2Dテクスチャ
		Microsoft::WRL::ComPtr<ID3D11Texture2D> texture2D;
		//深度ステンシル用2Dテクスチャ
		Microsoft::WRL::ComPtr<ID3D11Texture2D> depthTexture2D;
		//シェーダーリソースビュー
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderResourceView;
		//レンダリングターゲットビュー
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
		//深度ステンシルビュー
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> depthStencilView;
		//2Dテクスチャのフォーマット
		static const DXGI_FORMAT FORMAT;
		//ビューの大きさ
		D3DXVECTOR2 viewSize;
		//デバイス
		Microsoft::WRL::ComPtr<ID3D11Device> device;
	private:
		//2Dテクスチャ作成
		void CreateTexture2D();
		//シェーダーリソースビュー作成　
		void CreateShaderResourceView();
		//レンダリングターゲットビュー作成
		void CreateRenderTargetView();
		//深度ステンシルビュー作成
		void CreateDepthStencilView();
	};
};