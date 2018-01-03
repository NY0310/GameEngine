#include <vector>
#include "../ShaderManager.h"
#include "../FollowCamera.h"

class Campus
{
public:
private:
	struct InkData
	{
		D3DXVECTOR4 Color;
		ALIGN16 D3DXVECTOR2 Uv;
		ALIGN16 float Scale = 5.0f;
	//	ID3D11Buffer* vertexBuffer;
	};


	struct CampusVertex
	{
		D3DXVECTOR3 Pos; //位置
		D3DXVECTOR2 Tex; //テクスチャー座標
	};

public:
	Campus() = default;
	~Campus() = default;
	Campus& operator= (const Campus&) = delete;
	Campus(const Campus&) = delete;

	void CreateInk(D3DXVECTOR4& Color, D3DXVECTOR2& uv, float sclae);
	void CreateVertexBuffer();
	ID3D11ShaderResourceView*& GetInkTexSRV(){ return inkTexSRV; }
	HRESULT InitD3D();
	ID3D11ShaderResourceView*& GetInkNormalMap() { return inkNormalMap; }//ハイトマップ	
	void Render();
private:
	ID3D11Buffer* CreateVertexBuffer(InkData& inkdata);
	void InkRender();
	void DripRender();
	void InkRender(InkData& inkdata);
	void SetViewPort(ID3D11RenderTargetView * rtv);
	void ClearViewPort(ID3D11RenderTargetView * rtv);
	const D3DXVECTOR3& ChangeRegularDevice(const D3DXVECTOR3& position) { return D3DXVECTOR3(position.x * 2 - 1, (position.y * 2 - 1) * -1, 0); }
	const D3DXVECTOR2& ChangeRegularDevice(const D3DXVECTOR2& position) { return D3DXVECTOR2(position.x * 2 - 1, (position.y * 2 - 1) * -1); }

	/// <summary>
	/// 全てのインクをレンダリングするキャンパス
	/// </summary>
	ID3D11InputLayout* inkVertexLayout;//インクテクスチャ用頂点インプットレイアウト
	ID3D11Buffer* inkConstantBuffer;//インクテクスチャ用コンスタントバッファ
	ID3D11Texture2D* inkTex;				//インクを塗るテクスチャ
//	ID3D11Texture2D* inkTexDS;				//インクを塗るテクスチャ
	ID3D11RenderTargetView* inkTexRTV;//インクを塗るテクスチャTRV
	ID3D11DepthStencilView* inkDSTexDSV;//深度マップテクスチャー用DSのDSV	
	ID3D11VertexShader* inkVertexShader;//インクテクスチャ用バーテックスシェーダー
	ID3D11PixelShader* inkPixelShader;//インクテクスチャ用ピクセルシェーダー
	std::vector<InkData> inkData;//インクデータ配列
	ShaderManager shadermanager;//シェーダー関連
	FollowCamera* camera = FollowCamera::GetInstance();
	ID3D11Device* device = Devices::Get().Device().Get();//デバイス
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//デバイスコンテキスト
	ID3D11ShaderResourceView* inkTexture;//インクテクスチャ
	ID3D11ShaderResourceView* inkTexSRV;   //インクを塗るテクスチャのSRV
	ID3D11ShaderResourceView* oldinkTexSRV;
	ID3D11SamplerState* sampleLimear;//テクスチャサンプラ
	ID3D11ShaderResourceView* inkNormalMap;//ハイトマップ	

	ID3D11VertexShader* DripVertexShader;//インクテクスチャ用バーテックスシェーダー
	ID3D11PixelShader* DripPixelShader;//インクテクスチャ用ピクセルシェーダー


	ID3D11Buffer* oldVertexBuffer;
	ID3D11Texture2D* inkTex2;				//インクを塗るテクスチャ
	ID3D11RenderTargetView* inkTexRTV2;//インクを塗るテクスチャTRV
	ID3D11DepthStencilView* inkDSTexDSV2;//深度マップテクスチャー用DSのDSV	
	ID3D11ShaderResourceView* inkTexSRV2;   //インクを塗るテクスチャのSRV

};