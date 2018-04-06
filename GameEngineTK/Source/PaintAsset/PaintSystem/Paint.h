#pragma once
#include <windows.h>
#include <vector>
#include "../../NYLibrary/ShaderManager/ShaderManager.h"
#include "../../NYLibrary/Camera/FollowCamera.h"
#include "../../NYLibrary/Math/Math.h"
#include "../../NYLibrary/SimpleTexture/SimpleTextures.h"
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
		ALIGN16 float Scale = 5.0f;
		ID3D11Buffer* vertexBuffer;
	};


	struct PaintVertex
	{
		D3DXVECTOR3 Pos; //位置
		D3DXVECTOR2 Tex; //テクスチャー座標
	};

public:
	Paint(bool isPlane = false);
	~Paint();
	Paint& operator= (const Paint&) = delete;
	Paint(const Paint&) = delete;

	void CreateInk(D3DXVECTOR4 Color, D3DXVECTOR2 uv, float size);
	void CreateVertexBuffer();
	ID3D11ShaderResourceView** GetInkTexSRV();
	void Initialize();
	void ClearRenderConfig();
private:
	bool isPlane;
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


	ComPtr<ID3D11Buffer> dripVertexBuffer;//
	std::unique_ptr<SimpleTextures> textures;//テクスチャ
	std::unique_ptr<SimpleTextures> dripTextures;//テクスチャ


	static ComPtr<ID3D11VertexShader> updateVertexShader;//インクテクスチャ用バーテックスシェーダー
	static ComPtr<ID3D11PixelShader> updatePixelShader;//インクテクスチャ用ピクセルシェーダー

};