/// <summary>
/// �y�C���g�V�X�e����K��������OBJ�N���X
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include "../../NYLibrary/ObjModel/Obj.h"
#include "Paint.h"
#include "../PaintCollision/PaintCollision.h"


class PaintObj : public Obj
{
public:
	//�R���X�g���N�^
	PaintObj(LPSTR fileName,bool isPlane = false);
	//�����Ȃ��̃R���X�g���N�^�֎~
	PaintObj() = delete;
	//�f�X�g���N�^
	~PaintObj() = default;
	//�q���𐶐����A�q���Ƃ��ēo�^
	void CreateAddChild();
	//������
	void Initialize();
	//�`��
	void Render();
	//�C���N�`��
	void InkRender() { campus->Render(); }
	//�����蔻��Ăяo��
	void OnCollisiton(Collider* collider);
	//�L�����p�X�ɕt�������C���N���N���A����
	void ClearCampus() { campus->ClearCampus(); }
	//�C���N����������UV�l���Z�o
	D3DXVECTOR2 CalcInkCollisionUv(const Triangle& triangle, const D3DXVECTOR3 & inter);
private:
	//�C���N��h��L�����p�X
	std::shared_ptr<Paint> campus;
	//���ʂ�
	bool isPlane;
};




