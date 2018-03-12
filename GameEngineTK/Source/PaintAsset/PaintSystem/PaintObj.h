/// <summary>
/// OBJ�t�@�C���̓ǂݍ��ݕ`��
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include "../../NYLibrary/Obj/Obj.h"
#include "../../NYLibrary/Collision/Collision.h"
#include "Paint.h"
#include "../../NYLibrary/Input/MouseRay.h"
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
private:
	std::unique_ptr<Paint>campus;
	std::unique_ptr<PaintCollision>paintCollision;
};




