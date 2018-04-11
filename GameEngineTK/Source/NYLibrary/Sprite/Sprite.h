#pragma once
#include <memory>
#include "../ShaderManager/ShaderManager.h"
#include "../Camera/FollowCamera.h"
#include "../Device/Device.h"
#include "../SceneGraph/Node/GameObjectNode.h"
#include "../Math/Math.h"

namespace NYLibrary
{
	class Sprite : public NYLibrary::GameObjectNode
	{
	public:
		enum Dimension
		{
			Dimension2,
			Dimension3,
		};
		//�R���X�g���N�^
		Sprite(LPCWSTR FileName, Dimension dimension = Dimension2);
		//�f�X�g���N�^
		~Sprite();
		// �R�s�[�R���X�g���N�^�֎~
		Sprite(const Sprite&) = delete;
		// ����֎~
		Sprite& operator=(const Sprite&) = delete;
		//������
		void Initialize();
		//�摜�ǂݍ���
		HRESULT LoadTexture(LPCWSTR FileName);
		//�`��
		void Render();
		//���W��ݒ�
		void Set2DPosition(const D3DXVECTOR2& position) {
			vertexBufferPosition = D3DXVECTOR2(0, 0);
			if (vertexBufferPosition != position)
			{
				vertexBufferPosition = position;
				CreateVertexBuffer2D();
			}
		}
		//�����x�擾
		float GetTransparency()
		{
			return transparency;
		}
		//�����x�ݒ�
		void SetTransparency(float transparency) {
			this->transparency = transparency;
		}
		//�V�F�[�_�[���\�[�X�r���[��ݒ�
		void SetShaderResourceView(const ComPtr<ID3D11ShaderResourceView>& SRV) { texture = SRV; GetTextureSize(texture.Get()); }
		//�V�F�[�_�[���\�[�X�r���[��ݒ�
		const ComPtr<ID3D11ShaderResourceView>& GetShaderResourceView() { return texture; }
		//�X�P�[���ݒ�
		void SetScale2D(const D3DXVECTOR2& scale) {
			this->scale = scale;
			CreateVertexBuffer2D();
		}
		//�X�P�[���擾
		const D3DXVECTOR2& GetScale2D() { return scale; CreateVertexBuffer3D();}

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
			D3DXVECTOR4 color;
			ALIGN16 float transparency;
			ALIGN16 bool isUseColor;
		};
		//�R���X�^���g�o�b�t�@�[���Z�b�g����(3D�̂�)
		void SetConstantBuffer();

		//�e�N�X�`���̃T�C�Y���擾����
		void GetTextureSize(ID3D11ShaderResourceView* srv);
		HRESULT CreateVertexBuffer2D();
		HRESULT CreateVertexBuffer3D();
		//�o�[�e�b�N�X�o�b�t�@�[���쐬����

		ComPtr<ID3D11VertexShader> vertexShader;//�o�[�e�b�N�X�V�F�[�_
		ComPtr<ID3D11PixelShader> pixelShader;//�s�N�Z���V�F�[�_

		ComPtr<ID3D11Buffer> constantBuffer;

		ComPtr<ID3D11Buffer> vertexBuffer;//�o�[�e�b�N�X�o�b�t�@�[
		ComPtr<ID3D11InputLayout> vertexLayout;//���_�C���b�v�b�g���C�A�E�g

		ComPtr<ID3D11ShaderResourceView> texture;//�e�N�X�`���[
		ComPtr<ID3D11SamplerState> sampler;//�e�N�X�`���[�̃T���v���[


		int widthSize;//��
		int hightSize;//����
		D3DXVECTOR2 vertexBufferPosition;//�o�[�e�b�N�X�o�b�t�@�[�̍��W
		Dimension dimension;//�`�掟��
		float transparency;//�����x
		D3DXVECTOR2 scale;//�X�P�[��
	};
};