#pragma once
#include "../CD3DXMESH.h"

class ObjState
{
	public:
		//���z�f�X�g���N�^
		virtual ~ObjState() = default;
		//�������z�֐���錾
		virtual void Execute(CD3DXMESH* pPlayer) = 0;
};