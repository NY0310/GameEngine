#pragma once
#include <memory>
#include "../ShaderManager.h"
#include "../FollowCamera.h"
#include "../Device.h"
#include "../MatrixObject.h"


class Sprite
{
public:
	enum Dimension
	{
		Dimension2,
		Dimension3,
	};
	//�R���X�g���N�^
	Sprite();
	//�f�X�g���N�^
	~Sprite();
	// �R�s�[�R���X�g���N�^�֎~
	Sprite(const Sprite&) = delete;
	// ����֎~
	Sprite& operator=(const Sprite&) = delete;
	//������
	HRESULT Initialize();
	//�摜�ǂݍ���
	HRESULT LoadTexture(LPCWSTR FileName);
	//�`��
	void Render();
	//���W��ݒ�
	void Set2DPosition(const D3DXVECTOR2& position) {
		if (matrixObject->GetPosition() != position)
		{
			matrixObject->SetPosition(D3DXVECTOR3(position.x,position.y,0));
			CreateVertexBuffer();
		}
	}
	//���W��ݒ�
	void Set3DPosition(const D3DXVECTOR3& position) {
		matrixObject->SetPosition(position);
	}

	void SetVertexBufferPosition(const D3DXVECTOR3& position) {
			vertexBufferPosition = position;
			CreateVertexBuffer();
	}

	//�傫�����擾
	void SetScale(const D3DXVECTOR3& scale) {
		matrixObject->SetScale(scale);
	}
	//��]���擾
	void SetRotation(const D3DXVECTOR3& rotation) {
		matrixObject->SetRotation(rotation);
	}
private:
	//�o�[�e�b�N�X�V�F�[�_�[�ɑ���f�[�^
	struct VertexData
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 tex;
	};
	//�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
	struct CONSTANT_BUFFER
	{
		D3DXMATRIX wvp;
		ALIGN16 float viewPortWidth ;
		ALIGN16 float viewPortHight;
	};

	//�e�N�X�`���̃T�C�Y���擾����
	void GetTextureSize(ID3D11ShaderResourceView* srv);
	//�o�[�e�b�N�X�o�b�t�@�[���쐬����
	HRESULT CreateVertexBuffer();

	ComPtr<ID3D11VertexShader> vertexShader;//�o�[�e�b�N�X�V�F�[�_
	ComPtr<ID3D11PixelShader> pixelShader;//�s�N�Z���V�F�[�_

	ComPtr<ID3D11Buffer> constantBuffer;

	ComPtr<ID3D11Buffer> vertexBuffer;//�o�[�e�b�N�X�o�b�t�@�[
	ComPtr<ID3D11InputLayout> vertexLayout;//���_�C���b�v�b�g���C�A�E�g

	ComPtr<ID3D11ShaderResourceView> texture;//�e�N�X�`���[
	ComPtr<ID3D11SamplerState> sampler;//�e�N�X�`���[�̃T���v���[


	int widthHalfSize;//��
	int hightHalfSize;//����
	D3DXVECTOR3 vertexBufferPosition;//�o�[�e�b�N�X�o�b�t�@�[�̍��W
	Dimension dimension;//�`�掟��
	FollowCamera* camera = FollowCamera::GetInstance();
	std::unique_ptr<MatrixObject> matrixObject;
};