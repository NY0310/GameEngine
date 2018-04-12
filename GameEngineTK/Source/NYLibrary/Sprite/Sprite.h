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
		//�A�N�^�[��ݒ�
		void SetAnchorPoint(D3DXVECTOR2& anchorPoint) {
			Math::Clamp(1, 0, anchorPoint);
			this->anchorPoint = anchorPoint;
		}
		//�A�N�^�[���擾
		const D3DXVECTOR2& GetAnchorPoint() {
			return this->anchorPoint;
		}
	private:
		//�o�[�e�b�N�X�V�F�[�_�[�ɑ���f�[�^
		struct VertexData
		{
			D3DXVECTOR3 position;
			D3DXVECTOR2 tex;
		};
		//�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� 
		struct ConstantBuffer3D
		{
			D3DXMATRIX wvp;
			D3DXVECTOR4 color;
			ALIGN16 float transparency;
		};

		//�V�F�[�_�[�p�̃R
		struct ConstantBuffer2D 
		{
			D3DXMATRIX world;
			ALIGN16 float viewPortX;
			float viewPortY;
		};

		void SetConstantBuffer2D();
		void SetConstantBuffer3D();

		//�e�N�X�`���̃T�C�Y���擾����
		void GetTextureSize(ID3D11ShaderResourceView* srv);
		HRESULT CreateVertexBuffer2D();
		HRESULT CreateVertexBuffer3D();
		//�o�[�e�b�N�X�o�b�t�@�[���쐬����

		static ComPtr<ID3D11VertexShader> vertexShader2D;//�o�[�e�b�N�X�V�F�[�_
		static ComPtr<ID3D11VertexShader> vertexShader3D;//�o�[�e�b�N�X�V�F�[�_
		static ComPtr<ID3D11PixelShader> pixelShader;//�s�N�Z���V�F�[�_

		static ComPtr<ID3D11Buffer> constantBuffer3D;//3D�p�R���X�^���g�o�b�t�@�[
		static ComPtr<ID3D11Buffer> constantBuffer2D;//2D�p�R���X�^���g�o�b�t�@�[


		ComPtr<ID3D11Buffer> vertexBuffer;//�o�[�e�b�N�X�o�b�t�@�[
		static ComPtr<ID3D11InputLayout> vertexLayout;//���_�C���b�v�b�g���C�A�E�g

		ComPtr<ID3D11ShaderResourceView> texture;//�e�N�X�`���[
		ComPtr<ID3D11SamplerState> sampler;//�e�N�X�`���[�̃T���v���[


		int widthSize;//��
		int hightSize;//����
		Dimension dimension;//�`�掟��
		float transparency;//�����x
		D3DXVECTOR2 anchorPoint;//�A�N�^�[
	};
};