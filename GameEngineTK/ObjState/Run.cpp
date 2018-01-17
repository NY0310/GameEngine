#include "Run.h"

ObjState* Run::pState;

ObjState * Run::GetInstance()
{
	if (!pState)
		pState = new Run();
	return pState;
}

void Run::Execute(CD3DXMESH * player)
{
}

void Run::Dispose()
{
	if (!pState)delete pState;
}
