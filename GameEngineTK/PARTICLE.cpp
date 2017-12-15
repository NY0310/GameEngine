#include "PARTICLE.h"

//
//
//
PARTICLE::PARTICLE(int MaxParticle,D3DXVECTOR3& EmitPos)
{
	ZeroMemory(this,sizeof(PARTICLE));
	m_MaxParticle=MaxParticle;
	m_EmitPos=EmitPos;
	pPartArray =new PART[m_MaxParticle];

	for(int i=0;i<m_MaxParticle;i++)
	{
		float x = (rand() % 100 - 50) / (float)6;
		if (rand() % 1)
		{
			x += -1.0f;
		}
		pPartArray[i].Pos= D3DXVECTOR3(x,4,0 - rand()% 4);
		pPartArray[i].Dir=D3DXVECTOR3(/*2*((float)rand()/(float)RAND_MAX)-1*/1,(float)rand()/(float)-1,/*2*((float)rand()/(float)1)-1*/1);
		D3DXVec3Normalize(&pPartArray[i].Dir,&pPartArray[i].Dir);
		pPartArray[i].Speed= (float)rand()/5 / 1000000;
		pPartArray[i].BirthFrame=rand();
		pPartArray[i].Zrot = 0.0f;
	}
}
//
//
//
PARTICLE::~PARTICLE()
{
}

//
//
//
void PARTICLE::Run()
{
	m_Frame++;
	for(int i=0;i<m_MaxParticle;i++)
	{
		if(m_Frame>0)
		{
			pPartArray[i].Pos.y += pPartArray[i].Dir.y * pPartArray[i].Speed;
			//重力
			//pPartArray[i].Dir+=D3DXVECTOR3(0,-0.0000098,0);
			//////地面でのバウンド
			//if(pPartArray[i].Pos.y<0)
			//{
			//	pPartArray[i].Dir.y=-pPartArray[i].Dir.y;
			//}
			pPartArray[i].Zrot += 0.001f;
		}
	}
}
//
//
//
D3DXVECTOR3 PARTICLE::GetParticlePos(int Index)
{
	return pPartArray[Index].Pos;
}

float PARTICLE::GetParticleRot(int Index)
{
	return pPartArray[Index].Zrot;
}
