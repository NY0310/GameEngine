#include "Stage.h"

using namespace DirectX;
using namespace DirectX::SimpleMath;


LandShape Stage::m_LandShape;

Stage::Stage()
{
	
}


Stage::~Stage()
{
}

void Stage::Initialize()
{
	//�n�`�f�[�^�̓ǂݍ��� landshepe cmo�t�@�C����
	m_LandShape.Initialize(L"ground200m", L"ground200m");
	m_LandShape.SetRot(Vector3(0, 4.0f, 0));
}
