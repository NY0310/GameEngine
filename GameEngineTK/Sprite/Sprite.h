#pragma once
#include <memory>
#include "../ShaderManager.h"
#include "../FollowCamera.h"
#include "../Device.h"
#include "../MatrixObject.h"


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
	void Set2DPosition(const D3DXVECTOR2& position) {
		if (matrixObject->GetPosition() != position)
		{
			matrixObject->SetPosition(D3DXVECTOR3(position.x,position.y,0));
			CreateVertexBuffer();
		}
	}
	//座標を設定
	void Set3DPosition(const D3DXVECTOR3& position) {
		matrixObject->SetPosition(position);
	}

	void SetVertexBufferPosition(const D3DXVECTOR3& position) {
			vertexBufferPosition = position;
			CreateVertexBuffer();
	}

	//大きさを取得
	void SetScale(const D3DXVECTOR3& scale) {
		matrixObject->SetScale(scale);
	}
	//回転を取得
	void SetRotation(const D3DXVECTOR3& rotation) {
		matrixObject->SetRotation(rotation);
	}
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


	int widthHalfSize;//幅
	int hightHalfSize;//高さ
	D3DXVECTOR3 vertexBufferPosition;//バーテックスバッファーの座標
	Dimension dimension;//描画次元
	FollowCamera* camera = FollowCamera::GetInstance();
	std::unique_ptr<MatrixObject> matrixObject;
};