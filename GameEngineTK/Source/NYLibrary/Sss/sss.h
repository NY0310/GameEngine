#pragma once
#include "ShaderManager.h"
#include "Device.h"
#include "FollowCamera.h"


class sss
{
private:
	//頂点の構造体
	struct SIMPLE_VERTEX
	{
		D3DXVECTOR3 vPos;//位置
		D3DXVECTOR3 vNorm;//法線
	};

	//オリジナルメッシュ
	struct MY_MESH
	{
		DWORD dwNumVert;
		DWORD dwNumFace;
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
		D3DXMATRIX mWorld;
		MY_MESH()
		{
			ZeroMemory(this, sizeof(MY_MESH));
		}
	};
	
#define ALIGN16 _declspec(align(16))
	//
	struct CONSTANT_BUFFER_SSS
	{
		D3DXMATRIX mW;
		D3DXMATRIX mWVP;
		D3DXMATRIX mWLPT;
		D3DXMATRIX mWLP;
		D3DXVECTOR3 vLightPos;//ライトの位置
		ALIGN16 D3DXVECTOR4 Ambient;//アンビエント光
		D3DXVECTOR4 Diffuse; //拡散反射
		D3DXVECTOR4 Specular;//鏡面反射
		D3DXVECTOR3 vEye;
		ALIGN16 float g_Transparent;////どこまでの長さなら透過するか
	};
	//
	struct SHADER
	{
		ID3D11VertexShader* m_pVertexShader;
	};
	const int DEPTHTEX_WIDTH = 800 * 2;
	const int DEPTHTEX_HEIGHT = 600 * 2;


	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;

	ShaderManager& shadermanager = ShaderManager::Get();
	ID3D11Device* pDevice = Devices::Get().Device().Get();
	ID3D11DeviceContext* pDeviceContext = Devices::Get().Context().Get();
	float angle;
	float modelAngele;


	D3DXMATRIX mR; 
	D3DXMATRIX mLight;
	D3DXVECTOR3 vLight;
	D3D11_MAPPED_SUBRESOURCE pData;
public:
	sss();
	~sss();
	IDXGISwapChain* m_pSwapChain;

	//ID3D11RenderTargetView* m_pBackBuffer_TexRTV;//通常レンダリング用
	//ID3D11DepthStencilView* m_pBackBuffer_DSTexDSV;//通常レンダリング用
	//ID3D11Texture2D* m_pBackBuffer_DSTex;//通常レンダリング用
	
	ID3D11RenderTargetView* m_pDepthRTV;//深度テクスチャーレンダリング用
	ID3D11DepthStencilView* m_pDepthDSV;//深度テクスチャーレンダリング用
	ID3D11Texture2D* m_pDepthDepthStencil;//深度テクスチャーレンダリング用

	ID3D11RasterizerState* m_pRasterizerState;
	D3DXMATRIX m_mView;
	D3DXMATRIX m_mProj;
	ID3D11SamplerState* m_pSamLinear;
	MY_MESH* m_pMesh;
	D3DXVECTOR3 m_vLightPos;
	ID3D11Texture2D* m_pDepthTexture;
	ID3D11ShaderResourceView* m_pDepthTextureView;
	ID3D11InputLayout* m_pVertexLayout[2];
	ID3D11VertexShader* m_pVertexShader[2];
	ID3D11PixelShader* m_pPixelShader[2];
	ID3D11Buffer* m_pConstantBuffer[2];

	//method
	HRESULT InitD3D();
	void Destroy();
	void App();
	void Render(std::unique_ptr<FollowCamera>& camera);
	void ZTexRender(std::unique_ptr<FollowCamera>& camera);
	HRESULT InitShader();
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH* pMesh);

};

