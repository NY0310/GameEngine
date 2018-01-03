/// <summary>
/// OBJファイルの読み込み描画
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include "../ShaderManager.h"
#include "../FollowCamera.h"


class OBJAbstract
{
protected:

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

	using Vector3 = DirectX::SimpleMath::Vector3;
public:

	OBJAbstract();
	virtual ~OBJAbstract();
	//初期化処理
	void Init();
	HRESULT InitD3D();
	//OBJファイル読み込み
	void LoadOBJFile(LPSTR filename) { InitStaticMesh(filename, &mesh); }
	//テクスチャ読み込み
	void LoadTextuerFile(LPSTR filename) { D3DX11CreateShaderResourceViewFromFileA(device, filename, nullptr, nullptr, &texture, nullptr); }
	//描画
	virtual void Render();
	//Zテクスチャ描画
	virtual void ZTextureRender();
	//ワールド座標設定
	void SetPosition(const D3DXVECTOR3& position) { world = position;}
	//ワールド座標取得
	const D3DXVECTOR3& GetPosition() { return world; }
	//スケール設定
	void SetScale(const D3DXVECTOR3& scale) { this->scale = scale; }
	//スケール取得
	const D3DXVECTOR3& GetScale() { return  this->scale; }
	//スケール設定
	void SetRotation(const D3DXVECTOR3& rotation) { this->rotation = rotation; IsUseQuternion = false; }
	//スケール取得
	const D3DXVECTOR3& GetRotation() { return  this->rotation; }
	//クォータニオン設定
	void SetQuaternion(const D3DXQUATERNION& quaternion) { this->quaternion = quaternion; IsUseQuternion = true; }
	//クォータニオン取得
	const D3DXQUATERNION& GetQuaternion() { return  this->quaternion; }
	//ワールド座標設定
	void SetPosition(const Vector3& position) { world =  shadermanager.VectorToD3DXVECTOR3(position); }
	//ワールド座標取得
	const Vector3& GetPositionMath() { return shadermanager.D3DXVECTOR3ToVector(world); }
	//スケール設定
	void SetScale(const Vector3& scale) { this->scale = shadermanager.VectorToD3DXVECTOR3(scale); }
	//スケール取得
	const Vector3& GetScaleMath() { return shadermanager.D3DXVECTOR3ToVector(this->scale); }
	//回転設定
	void SetRotation(const Vector3& rotation) { this->rotation = shadermanager.VectorToD3DXVECTOR3(rotation); }
	//回転取得
	const Vector3& GetRotationMath() { return shadermanager.D3DXVECTOR3ToVector(this->rotation); IsUseQuternion = false;}
	//クォータニオン設定
	void SetQuaternion(const DirectX::SimpleMath::Quaternion& quaternion) { this->quaternion = shadermanager.QuaterniontoD3DXQUATERNION(quaternion); IsUseQuternion = true; }
	//クォータニオン取得
	const DirectX::SimpleMath::Quaternion& GetQuaternionMath() { return  shadermanager.D3DXQUATERNIONtoQuaternion(this->quaternion); }


private:
	//シェーダー作成
	HRESULT CreateShader();
	//深度マップシェーダー作成
	HRESULT CreateDepthTextureShader();
	//頂点インプットレイアウト作成
	HRESULT CreateVertexInputLayout(ID3DBlob * compiledshader);
	//コンスタントバッファ作成
	ID3D11Buffer* CreateConstantBuffer(UINT size);
	//深度マップテクスチャ作成
	HRESULT CreateDepthTexture();
	//サンプラー作成
	HRESULT CreateSampler();
	//OBJファイル読み込み
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);
	//マテリアルファイル読み込み
	HRESULT LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL * pMarial);
protected:
	//行列作成
	void AllMatrixCreate();

protected:
	/// <summary>
	/// メッシュ
	/// </summary>
	MY_MESH mesh;//メッシュ情報
	MY_MATERIAL material;//マテリアル構造体
	D3DXVECTOR3 world;//ワールド座標
	D3DXVECTOR3 scale;//サイズ
	D3DXVECTOR3 rotation;//回転
	D3DXQUATERNION quaternion;//クォータニオン
	bool IsUseQuternion;//クォータニオンを使用するか
	D3DXMATRIX worldMatrix;//ワールド行列
	D3DXMATRIX scaleMatrix;//スケール行列
	D3DXMATRIX rotationMatrix;//回転行列
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


	D3DXVECTOR3 m_vLight;



	/// <summary>
	/// 委譲
	/// </summary>
	ID3D11Device* device = Devices::Get().Device().Get();//デバイス
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//デバイスコンテキスト
	ShaderManager shadermanager = ShaderManager::Get();//シェーダー関連
	FollowCamera* camera = FollowCamera::GetInstance();

	//ビューポートサイズ
	const UINT DEPTHTEX_WIDTH = Devices::Get().Width() * 2;
	const UINT DEPTHTEX_HEIGHT = Devices::Get().Height() * 2;



	D3DXMATRIX m_mClipToUV;//テクスチャ行列
	D3DXVECTOR3 m_vLightPos;//ライド座標
	D3DXMATRIX mLight;//ライト行列
};



