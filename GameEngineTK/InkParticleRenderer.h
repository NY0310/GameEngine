#pragma once
#include <d3d11_1.h>
#include <SimpleMath.h>
#include "FollowCamera.h"
#include "Device.h"
#include "ShaderManager.h"
#include "AStar/Math.h"
#include "UsingDirectSimpleMath.h"


#pragma comment(lib, "d3dcompiler.lib")

using namespace UsingDirectSimpleMath;
using namespace Microsoft::WRL;



class InkParticleRenderer
{
public:
	//�R���X�^���g�o�b�t�@�[�o�̓f�[�^
	struct ConstantInkData
	{
		Vector3 positon;
		Vector3 scale;
		Vector3	rotation;
		Vector4 color;
	};
	HRESULT Initialize();//�������֐���S�Ď��s����
	void Render(const vector<ConstantInkData> inkData);//�S�p�[�e�B�N���̃��[���h�s����w�肵�ĕ`�揈�����Ă�
	InkParticleRenderer() = default;
	~InkParticleRenderer();
private:
	void RenderSprite(DirectX::SimpleMath::Matrix & WVP, DirectX::SimpleMath::Vector4& color);//�`�揈��

private:
	//���_�̍\����
	struct vertexData
	{
		D3DXVECTOR3 position; //�ʒu
		D3DXVECTOR2 uv; //�e�N�X�`���[���W
	};
	//�V�F�[�_�[�p�̃R���X�^���g�o�b�t�@�[�̃A�v�����\���� �������V�F�[�_�[���̃R���X�^���g�o�b�t�@�[�ƈ�v���Ă���K�v����
	struct CONSTANT_BUFFER
	{
		DirectX::SimpleMath::Matrix wvp;
		Vector4 color;
	};

	ComPtr<ID3D11VertexShader> vertexShader;//�o�[�e�b�N�X�V�F�[�_
	ComPtr<ID3D11GeometryShader> geometryShader;//�W�I���g���V�F�[�_
	ComPtr<ID3D11PixelShader> pixelShader;//�s�N�Z���V�F�[�_

	ComPtr<ID3D11Buffer> constantBuffer;

	ComPtr<ID3D11Buffer> vertexBuffer;//�o�[�e�b�N�X�o�b�t�@�[
	ComPtr<ID3D11InputLayout> vertexLayout;//���_�C���b�v�b�g���C�A�E�g

	ComPtr<ID3D11ShaderResourceView> texture;//�e�N�X�`���[
	ComPtr<ID3D11SamplerState> sampler;//�e�N�X�`���[�̃T���v���[
	ComPtr<ID3D11BlendState> blendState;//
	ComPtr<ID3D11DepthStencilState> depthStencilState;

	FollowCamera* camera = FollowCamera::GetInstance();

};

