/// <summary>
/// OBJファイルの読み込み描画
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include <Windows.h>
#include "ShaderManager.h"
#include "Device.h"
#include "FollowCamera.h"
#include "Collision.h"
#include "Line.h"
#include "Input/MouseUtil.h"
#include "Geometry.h"


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

	struct CampusVertex
	{
		D3DXVECTOR3 Pos; //位置
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


	struct InkData
	{
		D3DXVECTOR4 Color;
		ALIGN16 D3DXVECTOR2 Uv;
		ALIGN16 float Scale = 0.1f;
		ID3D11Buffer* vertexBuffer;
	};

	struct InkDataBuffer
	{
		D3DXVECTOR4 Color;
		ALIGN16 D3DXVECTOR2 Uv;
		ALIGN16 float Scale = 0.1f;
	};


	OBJ();
	~OBJ();
	void Init();
	HRESULT InitD3D();
	void CreateInkVertexBuffer(InkData & inkdata);
	HRESULT LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL * pMarial);
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);
	void Render(std::unique_ptr<FollowCamera>& camera);
	void InkRender(unique_ptr<FollowCamera>& camera);
	void InkRender(unique_ptr<FollowCamera>& camera, InkData & uv);
	void ZTextureRender(unique_ptr<FollowCamera>& camera);
	void Render(std::unique_ptr<FollowCamera>& camera, D3DXVECTOR3 && worldPosition);

	bool IntersectSegment(const Segment & segment, unique_ptr<FollowCamera>& camera);


	bool IntersectSphere(const Sphere & sphere, unique_ptr<FollowCamera>& camera);

	void MouseRay(unique_ptr<FollowCamera>& camera, unique_ptr<Player>&  player);

	DirectX::SimpleMath::Vector3 * CalcScreenToXZ(DirectX::SimpleMath::Vector3 * pout, int Sx, int Sy, int Screen_w, int Screen_h, DirectX::SimpleMath::Matrix * View, DirectX::SimpleMath::Matrix * Prj);


	DirectX::SimpleMath::Vector4 MatrixTimes(const DirectX::SimpleMath::Matrix & matrix, const DirectX::SimpleMath::Vector4 & vector);

	DirectX::SimpleMath::Vector3 * CalcScreenToWorld(DirectX::SimpleMath::Vector3 * pout, float Sx, float Sy, float fZ, float Screen_w, float Screen_h, DirectX::SimpleMath::Matrix * View, DirectX::SimpleMath::Matrix * Prj);

	void SetPosition(const D3DXVECTOR3& position);
//	const D3DXMATRIX& World() { return mW; }
private:
	ID3D11PixelShader* m_pPixelShader;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11HullShader* m_pHullShader;
	ID3D11DomainShader* m_pDomainShader;

	ID3D11Buffer* m_pConstantBuffer;
	ID3D11Buffer* m_pZTexConstantBuffer;//zテクスチャ用コンスタントバッファ



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



	D3DXMATRIX worldmat;
	DirectX::SimpleMath::Vector3 localPosition;
	std::vector<Triangle> triangles;


	std::vector<InkData> inkData;//インクデータ配列


	float DEPTHTEX_WIDTH = 800 * 2;
	float DEPTHTEX_HEIGHT = 600 * 2;


	float WINDOW_WIDTH = 800;
	float WINDOW_HEIGHT = 600;

	ID3D11Device* device = Devices::Get().Device().Get();
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();



	ID3D11Texture2D* m_pDepthMap_Tex;//深度マップテクスチャー
	ID3D11RenderTargetView* m_pDepthMap_TexRTV;//深度マップテクスチャーのRTV
	ID3D11ShaderResourceView* m_pDepthMap_TexSRV;//深度マップテクスチャーのSRV
	ID3D11Texture2D* m_pDepthMap_DSTex;//深度マップテクスチャー用DS
	ID3D11DepthStencilView* m_pDepthMap_DSTexDSV;//深度マップテクスチャー用DSのDSV	


	ID3D11VertexShader* m_pDepthVertexShader;//深度テクスチャーレンダリング用
	ID3D11PixelShader* m_pDepthPixelShader;//深度テクスチャーレンダリング用






	/// <summary>
	/// 全てのインクをレンダリング
	/// </summary>
	ID3D11InputLayout* inkVertexLayout;//インクテクスチャ用頂点インプットレイアウト
	ID3D11Buffer* inkConstantBuffer;//インクテクスチャ用コンスタントバッファ
	ID3D11Texture2D* inkTex;				//インクを塗るテクスチャ
	ID3D11Texture2D* inkTexDS;				//インクを塗るテクスチャ
	ID3D11RenderTargetView* inkTexRTV;//インクを塗るテクスチャTRV
	ID3D11ShaderResourceView* inkTexSRV;   //インクを塗るテクスチャのSRV
	ID3D11DepthStencilView* inkDSTexDSV;//深度マップテクスチャー用DSのDSV	
	ID3D11VertexShader* inkVertexShader;//インクテクスチャ用バーテックスシェーダー
	ID3D11PixelShader* inkPixelShader;//インクテクスチャ用ピクセルシェーダー


	D3DXMATRIX m_mClipToUV;//テクスチャ行列

	D3DXVECTOR3 m_vLightPos;//ライド座標
	D3DXMATRIX mLight;//ライト行列
	D3DXVECTOR3 ObjScale;//オブジェクトの座標


	std::unique_ptr<DirectX::Mouse> mouse;
	Mouse::ButtonStateTracker tracker;

	Line* line;


	D3DXMATRIX WVP;


	D3DXVECTOR3 world;


	const D3DXVECTOR3& ChangeRegularDevice(const D3DXVECTOR3& position) { return D3DXVECTOR3(position.x * 2 - 1,(position.y* 2 - 1) * -1, 0) ; }
};




