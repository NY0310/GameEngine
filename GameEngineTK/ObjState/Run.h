#pragma once
#include "ObjState.h"

class Run : public ObjState
{
	//Stand�N���X�̃C���X�^���X���擾����
	static ObjState* GetInstance();
	//���s����
	void Execute(CD3DXMESH* player);
	//Stand�I�u�W�F�N�g��j������
	static void Dispose();
private:
	static ObjState* pState;
};