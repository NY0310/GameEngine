/// <summary>
/// OBJファイルの読み込み描画
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include <memory>
#include <vector>
#include <map>
#include "../ShaderManager/ShaderManager.h"
#include "../SceneGraph/Node/GameObjectNode.h"
#include "../Light/Light.h"
#include "../../NYLibrary/Math/Lerp.h"

using namespace Microsoft::WRL;

namespace NYLibrary
{
	class Obj : public GameObjectNode
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
			D3DXMATRIX w;//ワールド行列
			D3DXMATRIX wvp;//ワールドから射影までの変換行列
			D3DXMATRIX wlp;//ワールド・”ライトビュー”・プロジェクションの合成
			D3DXMATRIX wlpt;//ワールド・”ライトビュー”・プロジェクション・テクスチャ座標行列の合成
			D3DXVECTOR4 lightDir;//ライト方向
			D3DXVECTOR4 eyesPos;//カメラ位置
			ALIGN16 float frame = 0.0f;
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
			std::vector<Triangle> triangles;
			MY_MESH mesh;
			D3DXVECTOR3 localSize;
		};

	public:
		Obj(LPSTR filename);
		Obj() = delete;
		virtual ~Obj();
		//オブジェクトを生成し子供として登録
		void CreateAddChild();
		//初期化処理
		virtual void Initialize();
		//OBJファイル読み込み
		void LoadOBJFile(LPSTR filename) { InitStaticMesh(filename, &mesh); }
		//テクスチャ読み込み
		void LoadTextuerFile(LPSTR filename) { D3DX11CreateShaderResourceViewFromFileA(device, filename, nullptr, nullptr, texture.GetAddressOf(), nullptr); }
		//描画
		void Render();
		//深度値の描画
		void ShadowRender();
		//破壊カウントを設定
		void SetBreakTime(float breakTime) {
	/*	breakLerp->SetTime(this->breakTime); */}
		//破壊を開始
		void BreakStart() { breakLerp->Start(); }
		void BreakClear() { breakLerp->Clear(); }
		//破壊が終わったか
		bool IsBreakEnd() { return breakLerp->IsLerpEnd(); }
		//破壊され終わったときに呼び出される
		virtual void OnBreakEnd(){}
		//破壊されたと判断される経過時間
		static const int BREAK_CNT = 60;

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
		static ComPtr<ID3D11VertexShader> vertexShader;//バッテックスシェーダー
		static ComPtr<ID3D11PixelShader> pixelShader;//ピクセルシェーダー
		static ComPtr<ID3D11GeometryShader> geometryShader;//ジオメトリシェーダー
		ComPtr<ID3D11Buffer> constantBuffer;//コンスタントバッファ
		static ComPtr<ID3D11InputLayout> vertexLayout;//頂点インプットレイアウト
		static ComPtr<ID3D11SamplerState> sampleLimear;//テクスチャサンプラ
		ComPtr<ID3D11ShaderResourceView> texture;//テクスチャ
		static std::map<LPSTR, MeshAndTriangles> modelDatas;//OBJモデル情報
		std::vector<Triangle> triangles;
		
		static const float MAX_BREAK_CNT;//破壊カウントの上限

		/// <summary>
		/// 委譲
		/// </summary>
		ID3D11Device* device = Devices::Get().Device().Get();//デバイス
		ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//デバイスコンテキスト
		FollowCamera* camera = FollowCamera::GetInstance();

		D3DXMATRIX clipToUV;//テクスチャ行列
		D3DXMATRIX mLight;//ライト行列
		std::shared_ptr<Lerp> breakLerp;//破壊のラープ
		static bool isFirst;
	};

};

