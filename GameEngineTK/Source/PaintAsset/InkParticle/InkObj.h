#pragma once
#include <vector>
#include "../../NYLibrary/ShaderManager/ShaderManager.h"
#include "../../NYLibrary/Object/LocalObject/LocalObject.h"
#include "../../NYLibrary/Camera/FollowCamera.h"

using namespace NYLibrary;
class InkObj
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


public:
	//コンスタントバッファー出力データ
	//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
	struct InkData
	{
		D3DXMATRIX wvp;//ワールドから射影までの変換行列
		D3DXVECTOR4 color;
	};
	InkObj();

	//初期化処理
	void Initialize();
	//OBJファイル読み込み
	void LoadOBJFile(LPSTR filename) { InitStaticMesh(filename, &mesh); }
	//描画
	void Render(vector<InkData> inkDataList);
	void Render(InkData inkdata);

private:
	//シェーダー作成
	HRESULT CreateShader();
	//頂点インプットレイアウト作成
	HRESULT CreateVertexInputLayout(ID3DBlob * compiledshader);
	//コンスタントバッファ作成
	ID3D11Buffer* CreateConstantBuffer(UINT size);
	//OBJファイル読み込み
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);
	/// <summary>
	/// メッシュ
	/// </summary>
	MY_MESH mesh;//メッシュ情報
	ComPtr<ID3D11VertexShader> vertexShader;//バッテックスシェーダー
	ComPtr<ID3D11PixelShader> pixelShader;//ピクセルシェーダー
	ComPtr<ID3D11Buffer> constantBuffer;//コンスタントバッファ
	ComPtr<ID3D11InputLayout> vertexLayout;//頂点インプットレイアウト

	std::vector<Triangle> triangles;



	ID3D11Device* device = NYLibrary::Devices::Get().Device().Get();//デバイス
	ID3D11DeviceContext* deviceContext = NYLibrary::Devices::Get().Context().Get();//デバイスコンテキスト
	NYLibrary::FollowCamera* camera = NYLibrary::FollowCamera::GetInstance();
};

