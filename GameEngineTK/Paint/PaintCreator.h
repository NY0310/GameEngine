#pragma once
#include <memory>
#include <SpriteFont.h>
#include "../InkParticle.h"
#include "../Input/MouseRay.h"
#include "../CollisionNode.h"
#include "../AStar/Math.h"
#include "../Sprite/Sprite.h"
#include "../Device.h"

class PaintCreator
{
public:
	PaintCreator();
	void Update();
	void Render();
private:
	MouseRay* mouseRay;
	InkParticleManager* inkParticleManager;
	std::unique_ptr<Sprite> aim;
};