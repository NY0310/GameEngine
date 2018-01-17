#include "Walk.h"

ObjState* Walk::pState;

ObjState * Walk::GetInstance()
{
	if (!pState)
		pState = new Walk();
	return pState;
}

void Walk::Execute(CD3DXMESH * player)
{
}

void Walk::Dispose()
{
	if (!pState)delete pState;
}
