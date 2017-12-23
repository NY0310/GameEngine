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
		ALIGN16 float Scale = 0.15f;
		ID3D11Buffer* vertexBuffer;
	};


	struct InkDataBuffer
	{
		D3DXVECTOR4 Color;
		ALIGN16 D3DXVECTOR2 Uv;
		ALIGN16 float Scale = 0.15f;
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
	ID3D11ShaderResourceView* inkTexSRV;   //インクを塗るテクスチャのSRV

	void CreateInk(D3DXVECTOR4& Color, D3DXVECTOR2& uv, float sclae);
	ID3D11ShaderResourceView* GetInkTexSRV(){ return inkTexSRV; }
	HRESULT InitD3D();
	void InkRender();
private:
	ID3D11Buffer* CreateVertexBuffer(InkData& inkdata);
	void InkRender(InkData& inkdata);
	const D3DXVECTOR3& ChangeRegularDevice(const D3DXVECTOR3& position) { return D3DXVECTOR3(position.x * 2 - 1, (position.y * 2 - 1) * -1, 0); }
	/// <summary>
	/// 全てのインクをレンダリングするキャンパス
	/// </summary>
	ID3D11InputLayout* inkVertexLayout;//インクテクスチャ用頂点インプットレイアウト
	ID3D11Buffer* inkConstantBuffer;//インクテクスチャ用コンスタントバッファ
	ID3D11Texture2D* inkTex;				//インクを塗るテクスチャ
	ID3D11Texture2D* inkTexDS;				//インクを塗るテクスチャ
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
	ID3D11SamplerState* sampleLimear;//テクスチャサンプラ
	ID3D11Buffer*  canvasVertexBuffer;
};