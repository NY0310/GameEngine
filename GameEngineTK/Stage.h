#pragma once

#include "LandShape.h"


class Stage
{
public:
	Stage();
	~Stage();
	void Initialize();
	void SetLandShape(LandShape& LandShape) { m_LandShape = std::move(LandShape); }
	LandShape& GetLandShape() { return m_LandShape; }

private:
	static LandShape m_LandShape;
};

