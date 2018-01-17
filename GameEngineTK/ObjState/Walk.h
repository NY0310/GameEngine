#pragma once
#include "ObjState.h"

class Walk : public ObjState
{
	//Standクラスのインスタンスを取得する
	static ObjState* GetInstance();
	//実行する
	void Execute(CD3DXMESH* player);
	//Standオブジェクトを破棄する
	static void Dispose();
private:
	static ObjState* pState;
}; 
