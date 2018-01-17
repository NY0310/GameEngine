/// <summary>
/// ���C�g
/// </summary>
#pragma once
#include <D3DX10.h>


class Light
{
public:
	//�R���X�g���N�^
	Light(D3DXVECTOR3 eyeposition);
	//�f�X�g���N�^
	~Light() = default;
	//�����Ȃ��̃R���X�g���N�^�֎~
	Light() = delete;
	//�R�s�[�R���X�g���N�^�֎~
	Light(const Light&) = delete;
	//����֎~
	Light& operator=(const Light&) = delete;
	//���C�g�r���[���擾
	const D3DXMATRIX GetView() { return view; }
private:
	D3DXMATRIX view;//�r���[
	D3DXVECTOR3 eyePosition;//���_
	D3DXVECTOR3 refPosition;//�����_
	D3DXVECTOR3 upVector;//������x�N�g��
};
