#pragma once
#include "../../NYLibrary/Sprite/Sprite.h"

class Aim : public NYLibrary::Sprite
{
public:
	Aim();
	~Aim() = default;
	void Update();
	void Render();
	void Initialize();
};