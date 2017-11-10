#pragma once
#include "ShaderManager.h"
#include "Device.h"
#include "FollowCamera.h"


#pragma once
class DisplacementMapping
{
private:
	//���_�̍\����
	struct SimpleVertex
	{
		D3DXVECTOR3 vPos;
		D3DXVECTOR2 vTex;
		D3DXVECTOR3 vNorm;
	};

	//�R���X�^���g�o�b�t�@�[
	struct SIMPLESHADER_CONSTANT_BUFFER
	{
		D3DXMATRIX mWVP;//���[���h�̃v���W�F�N�V�����s��
		D3DXMATRIX mW;//���[���h�s��
		ALIGN16 D3DXVECTOR3 vEyePos;//�e���_���猩���A���_�̈ʒu
		ALIGN16 float fMinDistance;
		ALIGN16 float fMaxDistance;
		ALIGN16 int iMaxDevide;
		ALIGN16 D3DXVECTOR4 LightDir;
		ALIGN16 D3DXVECTOR4 WaveMove;
	};

	//�I���W�i���@�}�e���A���\����
	struct MY_MATERIAL
	{
		CHAR szName[111];
		D3DXVECTOR4 Ka;//�A���r�G���g
		D3DXVECTOR4 Kd;//�f�B�t���[�Y
		D3DXVECTOR4 Ks;//�X�y�L�����[
		CHAR szTextureName[110];//�e�N�X�`���[�t�@�C����
	};



	//�I���W�i�����b�V��
	struct MY_MESH
	{
		DWORD dwNumVert;
		DWORD dwNumFace;
		ID3D11Buffer* pVertexBuffer;
		ID3D11Buffer* pIndexBuffer;
	};


	ShaderManager& shadermanager = ShaderManager::Get();
	ID3D11Device* pDevice = Devices::Get().Device().Get();
	ID3D11DeviceContext* pDeviceContext = Devices::Get().Context().Get();
	//�����f���̎�ނ���(���f���̍\�����S�ē���Ȃ�A�v���ɂЂƂj
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11HullShader* m_pHullShader;
	ID3D11DomainShader* m_pDomainShader;
	ID3D11PixelShader* m_pPixelShader;
	ID3D11Buffer* m_pConstantBuffer;
	//�����f������
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11SamplerState* m_pSamPoint;//�e�N�X�`���[�̃T���v���[
	ID3D11ShaderResourceView* m_pHeightTexture;//�e�N�X�`���[
	ID3D11ShaderResourceView* m_pDensityTexture;//�e�N�X�`���[
	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`���[

	MY_MESH m_Mesh;
	MY_MATERIAL m_Material;

	D3DXVECTOR4 WaveMove;
	D3DXVECTOR4 WaveHeight;

public:
	DisplacementMapping();
	~DisplacementMapping();
	HRESULT InitD3D();
	HRESULT InitShader();
	HRESULT InitPolygon();
	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH* pMesh);
	HRESULT LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL* pMaterial);

	void Render(std::unique_ptr<FollowCamera>& camera);

};

