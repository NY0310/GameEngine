#pragma once
#include "../ShaderManager/ShaderManager.h"
#include "../Device/Device.h"
#include "../Camera/FollowCamera.h"

using namespace Microsoft::WRL;
namespace NYLibrary
{
	class Line
	{
		//���_�̍\����
		struct SimpleVertex
		{
			D3DXVECTOR3 Pos; //�ʒu
		};
		//Simple�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
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