/// <summary>
/// OBJ�t�@�C���̓ǂݍ��ݕ`��
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include "../../NYLibrary/ObjModel/Obj.h"
//#include "../../NYLibrary/Collision/Collision.h"
#include "Paint.h"
//#include "../../NYLibrary/Input/MouseRay.h"
#include "../PaintCollision/PaintCollision.h"


class PaintObj : public Obj
{
public:
	PaintObj(LPSTR fileName,bool isPlane = false);
	PaintObj() = delete;
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
	D3DXVECTOR2 CalcInkCollisionUv(const Triangle& triangle, const D3DXVECTOR3 & inter);
private:
	//�C���N��h��L�����p�X
	std::shared_ptr<Paint>campus;
	//���ʉ�
	bool isPlane;
};




