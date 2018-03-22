#include "InkSegmentCollider.h"
#include "../../NYLibrary/Component/Element.h"

using namespace NYLibrary;
using namespace std;

InkSegmentCollider::InkSegmentCollider(const string & tag, ObjectData * matrixObject)
	:SegmentCollider(tag,matrixObject)
{
}

void InkSegmentCollider::Initialize()
{
	color = objectData->GetColor();
	SegmentCollider::Initialize();
}
