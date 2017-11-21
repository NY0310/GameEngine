/// <summary>
/// OBJファイルの読み込み描画
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include "ShaderManager.h"
#include "Device.h"
#include "FollowCamera.h"



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
		D3DXVECTOR4 inkColor;//インクの色
		ALIGN16 D3DXVECTOR2 inkUv;//インクテクスチャのUV座標
		float inkScale;//インクを塗る有効範囲
		//ALIGN16 bool ObjTexFlag;
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


	struct InkData
	{
		D3DXVECTOR4 Color;
		D3DXVECTOR2 Uv;
		 float Scale;
	};

	OBJ();
	~OBJ();
	void Init();
	HRESULT InitD3D();
	HRESULT LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL * pMarial);
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);
	void Render(std::unique_ptr<FollowCamera>& camera);
	void ZTextureRender(unique_ptr<FollowCamera>& camera);
	void Render(std::unique_ptr<FollowCamera>& camera, D3DXVECTOR3 && worldPosition);

	bool IntersectSegment(const Segment & segment, DirectX::SimpleMath::Vector3 * inter, unique_ptr<FollowCamera> camera);

	DirectX::SimpleMath::Vector4 MatrixTimes(const DirectX::SimpleMath::Matrix & matrix, const DirectX::SimpleMath::Vector3 & vector);


//	const D3DXMATRIX& World() { return mW; }
private:
	ID3D11PixelShader* m_pPixelShader;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11HullShader* m_pHullShader;
	ID3D11DomainShader* m_pDomainShader;

	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pZTexConstantBuffer;



	MY_MESH m_Mesh;
	ID3D11InputLayout* m_pVertexLayout;

	ShaderManager shadermanager;//シェーダー関連

	D3DXVECTOR3 m_vLight;

	ID3D11SamplerState* m_pSampleLimear;//テクスチャサンプラ
	ID3D11ShaderResourceView* m_pTexture;//テクスチャ
	int birthcnt;

	MY_MATERIAL m_Material;//マテリアル構造体

	ID3D11RasterizerState* m_pRasterizerState;


	ID3D11ShaderResourceView* inkTexture;//インクテクスチャ



	D3DXMATRIX World;
	DirectX::SimpleMath::Vector3 localPosition;
	std::vector<Triangle> triangles;



	InkData inkData;


	const int DEPTHTEX_WIDTH = 800 * 2;
	const int DEPTHTEX_HEIGHT = 600 * 2;


	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	ID3D11Device* device = Devices::Get().Device().Get();
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();



	ID3D11Texture2D* m_pDepthMap_Tex;//深度マップテクスチャー
	ID3D11RenderTargetView* m_pDepthMap_TexRTV;//深度マップテクスチャーのRTV
	ID3D11ShaderResourceView* m_pDepthMap_TexSRV;//深度マップテクスチャーのSRV
	ID3D11Texture2D* m_pDepthMap_DSTex;//深度マップテクスチャー用DS
	ID3D11DepthStencilView* m_pDepthMap_DSTexDSV;//深度マップテクスチャー用DSのDSV	


	ID3D11VertexShader* m_pDepthVertexShader;//深度テクスチャーレンダリング用
	ID3D11PixelShader* m_pDepthPixelShader;//深度テクスチャーレンダリング用
	D3DXMATRIX m_mClipToUV;

	D3DXVECTOR3 m_vLightPos;
	D3DXMATRIX mLight;
	;
};

