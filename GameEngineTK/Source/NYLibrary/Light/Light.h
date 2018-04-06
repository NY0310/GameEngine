/// <summary>
/// ���C�g
/// </summary>
#pragma once
#include <memory>
#include "../Object/MatrixObject/MatrixObject.h"

namespace NYLibrary
{
	class Light : public MatrixObject
	{
	public:
		//�f�X�g���N�^
		~Light() = default;
		//�R�s�[�R���X�g���N�^�֎~
		Light(const Light&) = delete;
		//����֎~
		Light& operator=(const Light&) = delete;
		static Light* GetInstance();
		////�X�V
		//void Update();
		//���C�g�r���[���擾
		const D3DXMATRIX& GetLightView() { return view; }
		//���C�g���_��ݒ�
		void SetLightEyePosition(const D3DXVECTOR3& eyePosition) { this->eyePosition = eyePosition; }
		//���C�g���_���擾
		D3DXVECTOR3 GetLightPosition() { return eyePosition; }
		//�s��v�Z
		void Calc();
	private:
		Light();
		//�����Ȃ��̃R���X�g���N�^
		static std::unique_ptr<Light> light;//���C�g
		D3DXMATRIX view;//�r���[
		D3DXVECTOR3 eyePosition;//���_
		D3DXVECTOR3 refPosition;//�����_
		D3DXVECTOR3 upVector;//������x�N�g��
	};
};