/// <summary>
/// OBJファイルの読み込み描画
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include <vector>
#include <map>
#include "../MatrixObject.h"
#include "../ShaderManager.h"
#include "../FollowCamera.h"
#include "../UsingDirectSimpleMath.h"

using namespace UsingDirectSimpleMath;

using namespace Microsoft::WRL;


class InkObj
{
private:

	//頂点の構造体
	struct SimpleVertex
	{
		D3DXVECTOR3 pos; //位置
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

	struct MeshAndTriangles
	{
		MY_MESH mesh;
	};

	//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
	struct ConstantBuffer
	{
		Matrix wvp;//ワールド行列
		Vector4 color;
	};
public:

	//コンスタントバッファー出力データ
	struct ConstantInkData
	{
		Vector3 positon;
		Vector3 scale;
		Vector3	rotation;
		Vector4 color;
	};

	InkObj();
	virtual ~InkObj();
	//初期化処理
	void Initialize();
	HRESULT InitD3D();
	//更新
	virtual  void Update();
	//OBJファイル読み込み
	void LoadOBJFile(LPSTR filename) { InitStaticMesh(filename, &mesh); }
	//描画
	void Render(const vector<ConstantInkData> inkData);
	void Render(Matrix & wvp, Vector4 & color);
	std::unique_ptr<MatrixObject> matrixInkObject;//行列

private:
	//シェーダー作成
	HRESULT CreateShader();
	//頂点インプットレイアウト作成
	HRESULT CreateVertexInputLayout(ID3DBlob * compiledshader);
	//コンスタントバッファ作成
	ID3D11Buffer* CreateConstantBuffer(UINT size);
	//サンプラー作成
	HRESULT CreateSampler();
	//OBJファイル読み込み
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);
	//マテリアルファイル読み込み
	HRESULT LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL * pMarial);
protected:
	/// <summary>
	/// メッシュ
	/// </summary>
	MY_MESH mesh;//メッシュ情報
	MY_MATERIAL material;//マテリアル構造体
	ComPtr<ID3D11VertexShader> vertexShader;//バッテックスシェーダー
	ComPtr<ID3D11PixelShader> pixelShader;//ピクセルシェーダー
	ComPtr<ID3D11Buffer> constantBuffer;//コンスタントバッファ
	ComPtr<ID3D11InputLayout> vertexLayout;//頂点インプットレイアウト
	ComPtr<ID3D11SamplerState> sampleLimear;//テクスチャサンプラ
	ComPtr<ID3D11ShaderResourceView> texture;//テクスチャ


	

	static std::map<LPSTR, MeshAndTriangles> modelDatas;//OBJモデル情報

	ID3D11Device* device = Devices::Get().Device().Get();//デバイス
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//デバイスコンテキスト
	FollowCamera* camera = FollowCamera::GetInstance();
	D3DXMATRIX mLight;//ライト行列
};



