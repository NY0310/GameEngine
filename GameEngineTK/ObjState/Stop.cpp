#include "Stop.h"

ObjState* Stop::pState;

ObjState * Stop::GetInstance()
{
	if (!pState)
		pState = new Stop();
	return pState;
}

void Stop::Execute(CD3DXMESH * player)
{
}

void Stop::Dispose()
{
	if (!pState)delete pState;
}
