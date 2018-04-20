#pragma once
#include <windows.h>
#include <vector>
#include "../../NYLibrary/ShaderManager/ShaderManager.h"
#include "../../NYLibrary/Camera/FollowCamera.h"
#include "../../NYLibrary/Math/Math.h"
#include "CampusTexture/CampusTextures.h"
#include "../../NYLibrary/SceneGraph/Node/GameObjectNode.h"

using namespace Microsoft::WRL;
using namespace NYLibrary;

class Paint : public GameObjectNodeEmpty
{
private:
	struct InkData
	{
		D3DXVECTOR4 Color;
		ALIGN16 D3DXVECTOR2 Uv;
		ALIGN16 float Size = 5.0f;
		ID3D11Buffer* vertexBuffer;
	};


	struct PaintVertex
	{
		D3DXVECTOR3 Pos; //位置
		D3DXVECTOR2 Tex; //テクスチャー座標
	};

public:
	//コンストラクタ
	Paint(bool isPlane = false);
	//デストラクタ
	~Paint();
	//代入禁止
	Paint& operator= (const Paint&) = delete;
	//コピーコンストラクタ禁止
	Paint(const Paint&) = delete;
	//付着したインクを生成
	void CreateInk(D3DXVECTOR4 Color, D3DXVECTOR2 uv, float size);
	//バーテックスバッファー作成
	void CreateVertexBuffer();
	//インクのシェーダリソースビューを取得
	ID3D11ShaderResourceView** Paint::GetInkTexSRV()
	{
		return dripTextures->GetShaderResourceView().GetAddressOf();
	}

	//初期化
	void Initialize();
	//インクの描画
	void ClearRenderConfig();
	//キャンパスに付着したインクをクリアする
	void ClearCampus();
private:
	ID3D11Buffer* CreateVertexBuffer(InkData& inkdata);
	void InkRender();
	void DripRender();
	void UpDateRender();
	void InkRender(InkData& inkdata);

	/// <summary>
	/// 全てのインクをレンダリングするキャンパス
	/// </summary>
	static ComPtr<ID3D11InputLayout> inkVertexLayout;//インクテクスチャ用頂点インプットレイアウト
	ComPtr<ID3D11Buffer> inkConstantBuffer;//インクテクスチャ用コンスタントバッファ
	static ComPtr<ID3D11VertexShader> inkVertexShader;//インクテクスチャ用バーテックスシェーダー
	static ComPtr<ID3D11PixelShader> inkPixelShader;//インクテクスチャ用ピクセルシェーダー



	std::vector<InkData> inkData;//インクデータ配列
	ID3D11Device* device = Devices::Get().Device().Get();//デバイス
	ComPtr<ID3D11ShaderResourceView> inkTexture;//インクテクスチャ
	ComPtr<ID3D11SamplerState> sampleLimear;//テクスチャサンプラ
	ComPtr<ID3D11ShaderResourceView> inkNormalMap;//ハイトマップ	

	static ComPtr<ID3D11VertexShader> DripVertexShader;//インクテクスチャ用バーテックスシェーダー
	static ComPtr<ID3D11PixelShader>  DripPixelShader;//インクテクスチャ用ピクセルシェーダー
	static ComPtr<ID3D11VertexShader> DripPlaneVertexShader;//インクテクスチャ用バーテックスシェーダー
	static ComPtr<ID3D11PixelShader>  DripPlanePixelShader;//インクテクスチャ用ピクセルシェーダー


	ComPtr<ID3D11Buffer> dripVertexBuffer;//垂れ流し用バーテックスバッファー
	std::unique_ptr<CampusTextures> textures;//テクスチャ
	std::unique_ptr<CampusTextures> dripTextures;//垂れたテクスチャ


	static ComPtr<ID3D11VertexShader> updateVertexShader;//垂れ流し更新用バーテックスシェーダー
	static ComPtr<ID3D11PixelShader> updatePixelShader;//垂れ流し更新用ピクセルシェーダー

	bool isPlane;//平面か
};