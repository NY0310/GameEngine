/// <summary>
/// OBJ�t�@�C���̓ǂݍ��ݕ`��
/// 2017/08/09
/// yusuke nakata
/// </summary>

#pragma once
#include "OBJAbstract.h"
#include "../Collision.h"
//#include "Ink.h"
//#include "../Input/MouseRay.h"
//#include "PaintCollision.h"


class OBJ : public OBJAbstract
{
public:
	OBJ();
	~OBJ() = default;
	//�X�V����
	void UpDate();
	//�`��
	void Render();
	//�C���N�`��
	void InkRender() {/* campus->Render();*/ }
private:
	//std::unique_ptr<Campus>campus;
//	std::unique_ptr<PaintCollision>paintCollision;
	//MouseRay* mouseRay;
};




