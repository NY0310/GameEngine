#pragma once
#include "ShaderManager.h"
#include "FollowCamera.h"

#include<d3dx9.h>
#include "ShaderManager.h"
#include "Device.h"

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"d3d9.lib")
#pragma comment(lib,"d3dx9.lib")

class BumpMapping
{
private:
	//頂点の構造体
	struct SimpleVertex
	{
		D3DXVECTOR3 Pos;//位置
		D3DXVECTOR3 Normal;//法線
		D3DXVECTOR2 vTex; //テクスチャー座標
		D3DXVECTOR4 vTangent; //タンジェント（接線）
	};
	//シェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
	struct SHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mW;//ワールド行列
		D3DXMATRIX mWIT;//ワールドの逆の転置行列
		D3DXMATRIX mWVP;//ワールドから射影までの変換行列
		D3DXVECTOR4 vLightDir;//ライト方向
		D3DXVECTOR4 Ambient;//環境光
		D3DXVECTOR4 Diffuse;//拡散反射光
		D3DXVECTOR4 Specular;//鏡面反射光
		D3DXVECTOR4 vEye;//カメラ位置（視点位置）
		D3DXVECTOR4 WaveMove;//波の動き
	};

	ShaderManager& shadermanager = ShaderManager::Get();
	ID3D11Device* pDevice = Devices::Get().Device().Get();
	ID3D11DeviceContext* pDeviceContext = Devices::Get().Context().Get();
public:
	BumpMapping();
	~BumpMapping();
	HRESULT InitD3D();
	HRESULT InitPolygon();
	HRESULT InitShader();
	void Render(std::unique_ptr<FollowCamera>& camera);

	IDXGISwapChain* m_pSwapChain;
	ID3D11RenderTargetView* m_pBackBuffer_TexRTV;
	ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;
	ID3D11Texture2D* m_pBackBuffer_DSTex;
	D3DXVECTOR3 m_vLight;

	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;

	ID3D11Buffer* m_pVertexBuffer;
	ID3D11SamplerState* m_pSampleLinear;//テクスチャーのサンプラー
	ID3D11Texture2D* m_pNormalTexture;//ノーマルマップのテクスチャー
	ID3D11ShaderResourceView* m_pNormalTextureSRV;//ノーマルマップのテクスチャーのSRV

};

