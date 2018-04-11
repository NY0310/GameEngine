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
		//座標を設定
		void Set2DPosition(const D3DXVECTOR2& position) {
			vertexBufferPosition = D3DXVECTOR2(0, 0);
			if (vertexBufferPosition != position)
			{
				vertexBufferPosition = position;
				CreateVertexBuffer2D();
			}
		}
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
		//スケール設定
		void SetScale2D(const D3DXVECTOR2& scale) {
			this->scale = scale;
			CreateVertexBuffer2D();
		}
		//スケール取得
		const D3DXVECTOR2& GetScale2D() { return scale; CreateVertexBuffer3D();}

	private:
		//バーテックスシェーダーに送るデータ
		struct VertexData
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 tex;
		};
		//シェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
		struct CONSTANT_BUFFER
		{
			D3DXMATRIX wvp;
			D3DXVECTOR4 color;
			ALIGN16 float transparency;
			ALIGN16 bool isUseColor;
		};
		//コンスタントバッファーをセットする(3Dのみ)
		void SetConstantBuffer();

		//テクスチャのサイズを取得する
		void GetTextureSize(ID3D11ShaderResourceView* srv);
		HRESULT CreateVertexBuffer2D();
		HRESULT CreateVertexBuffer3D();
		//バーテックスバッファーを作成する

		ComPtr<ID3D11VertexShader> vertexShader;//バーテックスシェーダ
		ComPtr<ID3D11PixelShader> pixelShader;//ピクセルシェーダ

		ComPtr<ID3D11Buffer> constantBuffer;

		ComPtr<ID3D11Buffer> vertexBuffer;//バーテックスバッファー
		ComPtr<ID3D11InputLayout> vertexLayout;//頂点インップットレイアウト

		ComPtr<ID3D11ShaderResourceView> texture;//テクスチャー
		ComPtr<ID3D11SamplerState> sampler;//テクスチャーのサンプラー


		int widthSize;//幅
		int hightSize;//高さ
		D3DXVECTOR2 vertexBufferPosition;//バーテックスバッファーの座標
		Dimension dimension;//描画次元
		float transparency;//透明度
		D3DXVECTOR2 scale;//スケール
	};
};