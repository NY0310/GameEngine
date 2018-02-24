#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>
#include "../ShaderManager.h"
#include "../FollowCamera.h"
#include "../Device.h"
#include "../UsingDirectSimpleMath.h"

using namespace  UsingDirectSimpleMath;

class Sprite
{
public:
	enum Dimension
	{
		Dimension2,
		Dimension3,
	};
	//コンストラクタ
	Sprite();
	//デストラクタ
	~Sprite();
	// コピーコンストラクタ禁止
	Sprite(const Sprite&) = delete;
	// 代入禁止
	Sprite& operator=(const Sprite&) = delete;
	//初期化
	HRESULT Initialize();
	//画像読み込み
	HRESULT LoadTexture(LPCWSTR FileName);
	//描画
	void Render();
	//座標を設定
	void SetPosition(const Vector3& position) {
		if (this->position != position)
		{
			this->position = position;
			CreateVertexBuffer();
		}
	}
	//大きさを取得
	void SetScale(const Vector3& scale) {
		this->scale = scale;
	}
	//回転を取得
	void SetRotation(const Vector3& rotation) {
		this->rotation = rotation;
	}
private:
	//バーテックスシェーダーに送るデータ
	struct VertexData
	{
		Vector3 position;
		Vector2 tex;
	};
	//シェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
	struct CONSTANT_BUFFER
	{
		DirectX::SimpleMath::Matrix wvp;
		ALIGN16 float viewPortWidth ;
		ALIGN16 float viewPortHight;
	};

	//テクスチャのサイズを取得する
	void GetTextureSize(ID3D11ShaderResourceView* srv);
	//バーテックスバッファーを作成する
	HRESULT CreateVertexBuffer();

	ComPtr<ID3D11VertexShader> vertexShader;//バーテックスシェーダ
	ComPtr<ID3D11PixelShader> pixelShader;//ピクセルシェーダ

	ComPtr<ID3D11Buffer> constantBuffer;

	ComPtr<ID3D11Buffer> vertexBuffer;//バーテックスバッファー
	ComPtr<ID3D11InputLayout> vertexLayout;//頂点インップットレイアウト

	ComPtr<ID3D11ShaderResourceView> texture;//テクスチャー
	ComPtr<ID3D11SamplerState> sampler;//テクスチャーのサンプラー

	Vector3 position;
	Vector3 scale;
	Vector3 rotation;

	int widthHalfSize;//幅
	int hightHalfSize;//高さ
	Dimension dimension;//描画次元
	FollowCamera* camera = FollowCamera::GetInstance();

};