#pragma once
#include <windows.h>
#include <vector>
#include "../../NYLibrary/ShaderManager/ShaderManager.h"
#include "../../NYLibrary/Camera/FollowCamera.h"
#include "../../NYLibrary/Math/Math.h"
#include "../../NYLibrary/SimpleTexture/SimpleTextures.h"
#include "../../NYLibrary/SceneGraph/Node/GameObjectNode.h"

using namespace Microsoft::WRL;
using namespace NYLibrary;

class Paint : public GameObjectNodeEmpty
{
private:
	struct InkData
	{
		D3DXVECTOR4 Color;
		ALIGN16 D3DXVECTOR2 Uv;
		ALIGN16 float Scale = 5.0f;
		ID3D11Buffer* vertexBuffer;
	};


	struct PaintVertex
	{
		D3DXVECTOR3 Pos; //�ʒu
		D3DXVECTOR2 Tex; //�e�N�X�`���[���W
	};

public:
	Paint(bool isPlane = false);
	~Paint();
	Paint& operator= (const Paint&) = delete;
	Paint(const Paint&) = delete;

	void CreateInk(D3DXVECTOR4 Color, D3DXVECTOR2 uv, float size);
	void CreateVertexBuffer();
	ID3D11ShaderResourceView** GetInkTexSRV();
	void Initialize();
	void ClearRenderConfig();
private:
	bool isPlane;
	ID3D11Buffer* CreateVertexBuffer(InkData& inkdata);
	void InkRender();
	void DripRender();
	void UpDateRender();
	void InkRender(InkData& inkdata);

	/// <summary>
	/// �S�ẴC���N�������_�����O����L�����p�X
	/// </summary>
	static ComPtr<ID3D11InputLayout> inkVertexLayout;//�C���N�e�N�X�`���p���_�C���v�b�g���C�A�E�g
	ComPtr<ID3D11Buffer> inkConstantBuffer;//�C���N�e�N�X�`���p�R���X�^���g�o�b�t�@
	static ComPtr<ID3D11VertexShader> inkVertexShader;//�C���N�e�N�X�`���p�o�[�e�b�N�X�V�F�[�_�[
	static ComPtr<ID3D11PixelShader> inkPixelShader;//�C���N�e�N�X�`���p�s�N�Z���V�F�[�_�[



	std::vector<InkData> inkData;//�C���N�f�[�^�z��
	ID3D11Device* device = Devices::Get().Device().Get();//�f�o�C�X
	ComPtr<ID3D11ShaderResourceView> inkTexture;//�C���N�e�N�X�`��
	ComPtr<ID3D11SamplerState> sampleLimear;//�e�N�X�`���T���v��
	ComPtr<ID3D11ShaderResourceView> inkNormalMap;//�n�C�g�}�b�v	

	static ComPtr<ID3D11VertexShader> DripVertexShader;//�C���N�e�N�X�`���p�o�[�e�b�N�X�V�F�[�_�[
	static ComPtr<ID3D11PixelShader>  DripPixelShader;//�C���N�e�N�X�`���p�s�N�Z���V�F�[�_�[
	static ComPtr<ID3D11VertexShader> DripPlaneVertexShader;//�C���N�e�N�X�`���p�o�[�e�b�N�X�V�F�[�_�[
	static ComPtr<ID3D11PixelShader>  DripPlanePixelShader;//�C���N�e�N�X�`���p�s�N�Z���V�F�[�_�[


	ComPtr<ID3D11Buffer> dripVertexBuffer;//
	std::unique_ptr<SimpleTextures> textures;//�e�N�X�`��
	std::unique_ptr<SimpleTextures> dripTextures;//�e�N�X�`��


	static ComPtr<ID3D11VertexShader> updateVertexShader;//�C���N�e�N�X�`���p�o�[�e�b�N�X�V�F�[�_�[
	static ComPtr<ID3D11PixelShader> updatePixelShader;//�C���N�e�N�X�`���p�s�N�Z���V�F�[�_�[

};