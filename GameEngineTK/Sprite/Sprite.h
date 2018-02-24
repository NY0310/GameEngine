#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>
#include "../ShaderManager.h"
#include "../FollowCamera.h"
#include "../Device.h"
#include "../UsingDirectSimpleMath.h"

using namespace  UsingDirectSimpleMath;

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
	void SetPosition(const Vector3& position) {
		if (this->position != position)
		{
			this->position = position;
			CreateVertexBuffer();
		}
	}
	//�傫�����擾
	void SetScale(const Vector3& scale) {
		this->scale = scale;
	}
	//��]���擾
	void SetRotation(const Vector3& rotation) {
		this->rotation = rotation;
	}
private:
	//�o�[�e�b�N�X�V�F�[�_�[�ɑ���f�[�^
	struct VertexData
	{
		Vector3 position;
		Vector2 tex;
	};
	//�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
	struct CONSTANT_BUFFER
	{
		DirectX::SimpleMath::Matrix wvp;
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

	Vector3 position;
	Vector3 scale;
	Vector3 rotation;

	int widthHalfSize;//��
	int hightHalfSize;//����
	Dimension dimension;//�`�掟��
	FollowCamera* camera = FollowCamera::GetInstance();

};