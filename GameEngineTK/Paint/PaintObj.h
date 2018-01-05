/// <summary>
/// OBJ�t�@�C���̓ǂݍ��ݕ`��
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include "Obj.h"
#include "../Collision.h"
#include "Ink.h"
#include "../Input/MouseRay.h"
#include "PaintCollision.h"


class PaintObj : public Obj
{
public:
	PaintObj();
	~PaintObj() = default;
	//�X�V����
	void UpDate();
	//�`��
	void Render();
	//�C���N�`��
	void InkRender() { campus->Render(); }
private:
	std::unique_ptr<Campus>campus;
	std::unique_ptr<PaintCollision>paintCollision;
	MouseRay* mouseRay;
};




