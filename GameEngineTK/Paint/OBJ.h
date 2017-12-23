/// <summary>
/// OBJファイルの読み込み描画
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include <Windows.h>
#include "../ShaderManager.h"
#include "../Device.h"
#include "../FollowCamera.h"
#include "../Collision.h"
#include "../Line.h"
#include "../Input/MouseUtil.h"
#include "../Geometry.h"
#include "Ink.h"


class Player;

class OBJ
{
public:
	//頂点の構造体
	struct SimpleVertex
	{
		D3DXVECTOR3 Pos; //位置
		D3DXVECTOR3 Normal;//法線
		D3DXVECTOR2 Tex; //テクスチャー座標
	};



	//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mW;//ワールド行列
		D3DXMATRIX mWVP;//ワールドから射影までの変換行列
		D3DXMATRIX mWLP;//ワールド・”ライトビュー”・プロジェクションの合成
		D3DXMATRIX mWLPT;//ワールド・”ライトビュー”・プロジェクション・テクスチャ座標行列の合成
		D3DXVECTOR4 vLightDir;//ライト方向
		D3DXVECTOR4 vEyes;//カメラ位置
		 //D3DXVECTOR4 vDiffuse;//ディヒューズ色
		 //D3DXVECTOR4 vSpecular;//鏡面反射光
	};



	struct ZTEXTURE_CONSTANT_BUFFER
	{
		D3DXMATRIX mWLP;//ワールドライト行列
	};

	//オリジナル　マテリアル構造体
	struct MY_MATERIAL
	{
		CHAR szName[111];
		D3DXVECTOR4 Ka;//アンビエント
		D3DXVECTOR4 Kd;//ディヒューズ
		D3DXVECTOR4 Ks;//スペキュラー
		CHAR szTextureName[110];//テクスチャーファイル名
	};


	//オリジナルメッシュ
	struct MY_MESH
	{
		DWORD dwNumVert;
		DWORD dwNumFace;
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
	};





	OBJ();
	~OBJ();
	void Init();
	HRESULT InitD3D();
	HRESULT LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL * pMarial);
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);
	void Render();
	void InkRender();
	void ZTextureRender();
	void Render(D3DXVECTOR3 && worldPosition);

	bool IntersectSegment(const Segment & segment,D3DXVECTOR2& uv);
	bool IntersectSphere(const Sphere & sphere);

	void MouseRay(unique_ptr<Player>&  player);

	DirectX::SimpleMath::Vector3 * CalcScreenToXZ(DirectX::SimpleMath::Vector3 * pout, int Sx, int Sy, int Screen_w, int Screen_h, DirectX::SimpleMath::Matrix * View, DirectX::SimpleMath::Matrix * Prj);


	DirectX::SimpleMath::Vector4 MatrixTimes(const DirectX::SimpleMath::Matrix & matrix, const DirectX::SimpleMath::Vector4 & vector);

	DirectX::SimpleMath::Vector3 * CalcScreenToWorld(DirectX::SimpleMath::Vector3 * pout, float Sx, float Sy, float fZ, float Screen_w, float Screen_h, DirectX::SimpleMath::Matrix * View, DirectX::SimpleMath::Matrix * Prj);

	void SetPosition(const D3DXVECTOR3& position);
//	const D3DXMATRIX& World() { return mW; }
private:
	/// <summary>
	/// メッシュ
	/// </summary>
	MY_MESH mesh;//メッシュ情報
	MY_MATERIAL material;//マテリアル構造体
	D3DXVECTOR3 world;//ワールド座標
	D3DXVECTOR3 ObjScale;//サイズ
	ID3D11VertexShader* vertexShader;//バッテックスシェーダー
	ID3D11PixelShader* pixelShader;//ピクセルシェーダー
	ID3D11Buffer* constantBuffer;//コンスタントバッファ
	ID3D11InputLayout* vertexLayout;//頂点インプットレイアウト
	ID3D11SamplerState* sampleLimear;//テクスチャサンプラ
	ID3D11ShaderResourceView* texture;//テクスチャ
	std::vector<Triangle> triangles;//三角形ポリゴン


	/// <summary>
	/// Zテクスチャ
	/// </summary>
	ID3D11Buffer* zTexConstantBuffer;//zテクスチャ用コンスタントバッファ
	ID3D11VertexShader* depthVertexShader;//深度テクスチャーレンダリング用
	ID3D11PixelShader* depthPixelShader;//深度テクスチャーレンダリング用
	ID3D11Texture2D* depthMapTex;//深度マップテクスチャー
	ID3D11RenderTargetView* depthMapTexRTV;//深度マップテクスチャーのRTV
	ID3D11ShaderResourceView* depthMapTexSRV;//深度マップテクスチャーのSRV
	ID3D11Texture2D* depthMapDSTex;//深度マップテクスチャー用DS
	ID3D11DepthStencilView* depthMapDSTexDSV;//深度マップテクスチャー用DSのDSV	



	/// <summary>
	/// モジュール
	/// </summary>
	ID3D11Device* device = Devices::Get().Device().Get();//デバイス
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//デバイスコンテキスト
	ShaderManager shadermanager;//シェーダー関連
	Line* line;//線
	FollowCamera* camera = FollowCamera::GetInstance();
	std::unique_ptr<Campus>campus;

	//ビューポートサイズ
	const UINT DEPTHTEX_WIDTH = Devices::Get().Width() * 2;
	const UINT DEPTHTEX_HEIGHT = Devices::Get().Height() * 2;


	D3DXVECTOR3 m_vLight;





	D3DXMATRIX worldMatrix;
	DirectX::SimpleMath::Vector3 localPosition;


	D3DXMATRIX m_mClipToUV;//テクスチャ行列
	D3DXVECTOR3 m_vLightPos;//ライド座標
	D3DXMATRIX mLight;//ライト行列



	


};




