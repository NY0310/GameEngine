#pragma once
//ヘッダーファイルのインクルード
#include <windows.h>
#include "ShaderManager.h"
//#include <d3d11.h>
//#include <d3dx10.h>
//#include <d3dx11.h>
//
//
struct PART
{
	D3DXVECTOR3 Pos;
	D3DXVECTOR3 Dir;
	float Speed;
	int BirthFrame;

	PART()
	{
		ZeroMemory(this,sizeof(PART));
	}
};
//
//
//
class PARTICLE
{
public:
	PARTICLE(int MaxParticle,D3DXVECTOR3& EmitPos);
	~PARTICLE();
	void Run();
	D3DXVECTOR3 GetParticlePos(int Index);

	int m_MaxParticle;
	D3DXVECTOR3 m_EmitPos;
	PART* pPartArray;
	int m_Frame;
};