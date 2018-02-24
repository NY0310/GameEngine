#include "PaintCreator.h"

using namespace std;
PaintCreator::PaintCreator()
{
	mouseRay = MouseRay::GetInstance();
	inkParticleManager = new InkParticleManager();
	inkParticleManager->Initialize();
	aim = make_unique<Sprite>();
	aim->Initialize();
	aim->LoadTexture(L"Resources/PNG/Aim.png");

}

void PaintCreator::Update()
{
	Segment* segment = mouseRay->RayCreate();
	Vector4 color[7]{
		Colors::Blue,
		Colors::Red,
		Colors::Green,
		Colors::Yellow,
		Colors::Purple,
		Colors::Aqua,
		Colors::Orange
	};
	MouseUtil* mouse = MouseUtil::GetInstance();
	auto& devices = Devices::Get();
	aim->SetPosition(Vector3(mouse->GetX() - (int)devices.Width() / 2,(int)devices.Height() / 2 - mouse->GetY(), 0));
	//inkParticleManager->Shoot(Vector3(0,0,0), Vector3(mouse->GetX(),mouse->GetY(),10) - Vector3::Zero, color[0]);

		static int a = 0;

	if (segment)
	{
		a++;
		inkParticleManager->Shoot(Vector3(segment->End), Vector3(segment->Start - segment->End), color[a]);
		delete segment;
	}
	inkParticleManager->Update();
}

void PaintCreator::Render()
{
	inkParticleManager->Render();
	aim->Render();
}
