#include "Stage.h"
#include "../../../NYLibrary/Input/KeyboardUtil.h"
using namespace std;


Stage::Stage()
	:PaintObj("Resources/OBJMODEL/Dome.obj",false)
{
	LoadTextuerFile("Resources/JPG/Summer.jpg");
	SetTag("stage");
}

void Stage::CreateAddChild()
{
	PaintObj::CreateAddChild();
	floor = make_shared<Floor>();
	AddChild(floor);
}


void Stage::Update()
{
	auto& a = Light::GetInstance()->GetLightPosition();


	auto key = KeyboardUtil::GetInstance();
	D3DXVECTOR3 tra(0,0,0);
	if (key->IsPressed(Keyboard::I))
		tra.y = 1;
		if (key->IsPressed(Keyboard::K))
			tra.y -= 1;
			if (key->IsPressed(Keyboard::L))
				tra.x = 1;
				if (key->IsPressed(Keyboard::J))
					tra.x -= 1;
				if (key->IsPressed(Keyboard::N))
					tra.z -= 1;

					if (key->IsPressed(Keyboard::M))
						tra.z = 1;
					SetPosition(a + tra);
					Light::GetInstance()->SetLightEyePosition(a + tra);

}

void Stage::ShadowRender()
{

}

void Stage::Finalize()
{
}

Stage::~Stage()
{
}

