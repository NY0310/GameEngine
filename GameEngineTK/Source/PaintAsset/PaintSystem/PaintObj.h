/// <summary>
/// OBJ�t�@�C���̓ǂݍ��ݕ`��
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include "../../NYLibrary/Obj/Obj.h"
//#include "../../NYLibrary/Collision/Collision.h"
#include "Paint.h"
//#include "../../NYLibrary/Input/MouseRay.h"
#include "../PaintCollision/PaintCollision.h"


class PaintObj : public Obj
{
public:
	PaintObj(bool isPlane = false);
	~PaintObj() = default;
	//�X�V����
	void Update();
	//�`��
	void Render();
	//�C���N�`��
	void InkRender() { campus->Render(); }
	//�����蔻��Ăяo��
	void OnCollisiton(Collider* collider);
	const D3DXVECTOR2& CalcInkCollisionUv(const Segment& segment, const Triangle& triangle, const D3DXVECTOR3 & inter);
private:
	//�C���N����������UV�l���Z�o����
	std::unique_ptr<Paint>campus;
	std::unique_ptr<PaintCollision>paintCollision;
};




