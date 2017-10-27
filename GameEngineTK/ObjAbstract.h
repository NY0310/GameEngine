//#pragma once
//#include "Device.h"
//#include "ShaderManager.h"
//
//class ObjAbstract
//{
//protected:
//
//
//
//
//	//�I���W�i�����b�V��
//	struct MY_MESH
//	{
//		DWORD dwNumVert;
//		DWORD dwNumFace;
//		ID3D11Buffer* pVertexBuffer;
//		ID3D11Buffer* pIndexBuffer;
//		D3DXMATRIX mWorld;
//	};
//
//
//	//�I���W�i���@�}�e���A���\����
//	struct MY_MATERIAL
//	{
//		CHAR szName[111];
//		D3DXVECTOR4 Ka;//�A���r�G���g
//		D3DXVECTOR4 Kd;//�f�B�q���[�Y
//		D3DXVECTOR4 Ks;//�X�y�L�����[
//		CHAR szTextureName[110];//�e�N�X�`���[�t�@�C����
//	};
//public:
//	ObjAbstract();
//	~ObjAbstract();
//
//protected:
//	HRESULT InitStaticMesh(LPSTR FileName, MY_MESH * pMesh);
//	HRESULT LoadMaterialFromFile(LPSTR FileName, MY_MATERIAL * pMarial);
//
//	ID3D11Device* pDevice = Devices::Get().Device().Get();
//	ID3D11DeviceContext* pDeviceContext = Devices::Get().Context().Get();
//
//	MY_MESH m_Mesh;
//	MY_MATERIAL m_Material;//�}�e���A���\����
//	ID3D11SamplerState* m_pSampleLimear;//�e�N�X�`���T���v��
//	ID3D11ShaderResourceView* m_pTexture;//�e�N�X�`��
//
//
//};
//
