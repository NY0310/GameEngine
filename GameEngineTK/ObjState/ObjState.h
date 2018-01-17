#pragma once
#include "../CD3DXMESH.h"

class ObjState
{
	public:
		//仮想デストラクタ
		virtual ~ObjState() = default;
		//純粋仮想関数を宣言
		virtual void Execute(CD3DXMESH* pPlayer) = 0;
};