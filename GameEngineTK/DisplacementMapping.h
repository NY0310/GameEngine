#pragma once
#include "ShaderManager.h"
#include "Device.h"
#include "FollowCamera.h"


#pragma once
class DisplacementMapping
{
private:
	//頂点の構造体
	struct SimpleVertex
	{
		D3DXVECTOR3 Pos;//位置
		D3DXVECTOR2 UV;
	};

	//コンスタントバッファー
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mWVP;//ワールドのプロジェクション行列
		D3DXVECTOR4 vLightDir;//ライト方向
	};




	ShaderManager& shadermanager = ShaderManager::Get();
	ID3D11Device* pDevice = Devices::Get().Device().Get();
	ID3D11DeviceContext* pDeviceContext = Devices::Get().Context().Get();

public:
	DisplacementMapping();
	~DisplacementMapping();
	HRESULT InitD3D();
	HRESULT InitShader();
	HRESULT InitPolygon();
	void Render(std::unique_ptr<FollowCamera>& camera);
	//↓モデルの種類ごと(モデルの構造が全て同一ならアプリにひとつ）
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11HullShader* m_pHullShader;
	ID3D11DomainShader* m_pDomainShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	//↓モデルごと
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11SamplerState* m_pSamPoint;//テクスチャーのサンプラー
	ID3D11ShaderResourceView* m_pHeightTexture;//テクスチャー

};

