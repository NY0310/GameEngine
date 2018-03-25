#pragma once
#include "../ShaderManager/ShaderManager.h"
#include "../Device/Device.h"
#include "../Camera/FollowCamera.h"

using namespace Microsoft::WRL;
namespace NYLibrary
{
	class Line
	{
		//頂点の構造体
		struct SimpleVertex
		{
			D3DXVECTOR3 Pos; //位置
		};
		//Simpleシェーダー用のコンスタントバッファーのアプリ側構造体 もちろんシェーダー内のコンスタントバッファーと一致している必要あり
		struct SIMPLESHADER_CONSTANT_BUFFER
		{
			D3DXMATRIX mWVP;
		};
	public:
		Line();
		~Line();
		HRESULT InitD3D();
		HRESULT InitModel();
		void Render();
		void SetVertex(D3DXVECTOR3 pos, D3DXVECTOR3 po1);
	private:
		ID3D11Device* device = Devices::Get().Device().Get();
		ID3D11DeviceContext* deviceContext = Devices::Get().Context().Get();
		ID3D11InputLayout* m_pVertexLayout;
		ID3D11VertexShader* m_pVertexShader;
		ID3D11PixelShader* m_pPixelShader;
		ID3D11Buffer* m_pConstantBuffer;
		ComPtr<ID3D11Buffer> m_pVertexBuffer;
		FollowCamera* camera = FollowCamera::GetInstance();

	};

}