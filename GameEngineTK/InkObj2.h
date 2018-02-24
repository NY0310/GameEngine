#pragma once
#include <vector>
#include "MatrixObject.h"
#include "ShaderManager.h"
#include "FollowCamera.h"


class InkObj2
{
protected:
	//頂点の構造体
	struct SimpleVertex
	{
		D3DXVECTOR3 Pos; //位置
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
	};


public:
	//コンスタントバッファー出力データ
	//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
	struct InkData
	{
		DirectX::SimpleMath::Matrix wvp;//ワールドから射影までの変換行列
		DirectX::SimpleMath::Vector4 color;
	};
	InkObj2();

	//初期化処理
	void Initialize();
	HRESULT InitD3D();
	//OBJファイル読み込み
	void LoadOBJFile(LPSTR filename) { InitStaticMesh(filename, &mesh); }
	//描画
	void Render(vector<InkData> inkDataList);
	void Render(InkData inkdata);
	std::unique_ptr<MatrixObject> matrixObject = std::make_unique<MatrixObject>();//行列

private:
	//シェーダー作成
	HRESULT CreateShader();
	//頂点インプットレイアウト作成
	HRESULT CreateVertexInputLayout(ID3DBlob * compiledshader);
	//コンスタントバッファ作成
	ID3D11Buffer* CreateConstantBuffer(UINT size);
	//OBJファイル読み込み
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);

protected:
	/// <summary>
	/// メッシュ
	/// </summary>
	MY_MESH mesh;//メッシュ情報
	ComPtr<ID3D11VertexShader> vertexShader;//バッテックスシェーダー
	ComPtr<ID3D11PixelShader> pixelShader;//ピクセルシェーダー
	ComPtr<ID3D11Buffer> constantBuffer;//コンスタントバッファ
	ComPtr<ID3D11InputLayout> vertexLayout;//頂点インプットレイアウト



	static std::map<LPSTR, MeshAndTriangles> modelDatas;//OBJモデル情報
	std::vector<Triangle> triangles;



	ID3D11Device* device = Devices::Get().Device().Get();//デバイス
	ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();//デバイスコンテキスト
	FollowCamera* camera = FollowCamera::GetInstance();


};

