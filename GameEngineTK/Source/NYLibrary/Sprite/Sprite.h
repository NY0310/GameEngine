#pragma once
#include <memory>
#include "../ShaderManager/ShaderManager.h"
#include "../Camera/FollowCamera.h"
#include "../Device/Device.h"
#include "../SceneGraph/Node/GameObjectNode.h"
#include "../Math/Math.h"

namespace NYLibrary
{
	class Sprite : public NYLibrary::GameObjectNode
	{
	public:
		enum Dimension
		{
			Dimension2,
			Dimension3,
		};
		//コンストラクタ
		Sprite(LPCWSTR FileName, Dimension dimension = Dimension2);
		//デストラクタ
		~Sprite();
		// コピーコンストラクタ禁止
		Sprite(const Sprite&) = delete;
		// 代入禁止
		Sprite& operator=(const Sprite&) = delete;
		//初期化
		void Initialize();
		//画像読み込み
		HRESULT LoadTexture(LPCWSTR FileName);
		//描画
		void Render();
		//透明度取得
		float GetTransparency()
		{
			return transparency;
		}
		//透明度設定
		void SetTransparency(float transparency) {
			this->transparency = transparency;
		}
		//シェーダーリソースビューを設定
		void SetShaderResourceView(const ComPtr<ID3D11ShaderResourceView>& SRV) { texture = SRV; GetTextureSize(texture.Get()); }
		//シェーダーリソースビューを設定
		const ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() { return texture; }
		//アクターを設定
		void SetAnchorPoint(D3DXVECTOR2& anchorPoint) {
			Math::Clamp(1, 0, anchorPoint);
			this->anchorPoint = anchorPoint;
		}
		//アクターを取得
		const D3DXVECTOR2& GetAnchorPoint() {
			return this->anchorPoint;
		}
	private:
		//バーテックスシェーダーに送るデータ
		struct VertexData
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 tex;
		};
		//シェーダー用のコンスタントバッファーのアプリ側構造体 
		struct ConstantBuffer3D
		{
			D3DXMATRIX wvp;
			D3DXVECTOR4 color;
			ALIGN16 float transparency;
		};

		//シェーダー用のコ
		struct ConstantBuffer2D 
		{
			D3DXMATRIX world;
			ALIGN16 float viewPortX;
			float viewPortY;
		};

		void SetConstantBuffer2D();
		void SetConstantBuffer3D();

		//テクスチャのサイズを取得する
		void GetTextureSize(ID3D11ShaderResourceView* srv);
		HRESULT CreateVertexBuffer2D();
		HRESULT CreateVertexBuffer3D();
		//バーテックスバッファーを作成する

		static ComPtr<ID3D11VertexShader> vertexShader2D;//バーテックスシェーダ
		static ComPtr<ID3D11VertexShader> vertexShader3D;//バーテックスシェーダ
		static ComPtr<ID3D11PixelShader> pixelShader;//ピクセルシェーダ

		static ComPtr<ID3D11Buffer> constantBuffer3D;//3D用コンスタントバッファー
		static ComPtr<ID3D11Buffer> constantBuffer2D;//2D用コンスタントバッファー


		ComPtr<ID3D11Buffer> vertexBuffer;//バーテックスバッファー
		static ComPtr<ID3D11InputLayout> vertexLayout;//頂点インップットレイアウト

		ComPtr<ID3D11ShaderResourceView> texture;//テクスチャー
		ComPtr<ID3D11SamplerState> sampler;//テクスチャーのサンプラー


		int widthSize;//幅
		int hightSize;//高さ
		Dimension dimension;//描画次元
		float transparency;//透明度
		D3DXVECTOR2 anchorPoint;//アクター
	};
};