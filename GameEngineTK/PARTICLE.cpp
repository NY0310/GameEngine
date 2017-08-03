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
		pPartArray[i].Pos=EmitPos;
		pPartArray[i].Dir=D3DXVECTOR3(2*((float)rand()/(float)RAND_MAX)-1,(float)rand()/(float)RAND_MAX,2*((float)rand()/(float)RAND_MAX)-1);
		D3DXVec3Normalize(&pPartArray[i].Dir,&pPartArray[i].Dir);
		pPartArray[i].Speed=(5+((float)rand()/(float)RAND_MAX))*0.0001;
		pPartArray[i].BirthFrame=rand();
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
			pPartArray[i].Pos+=pPartArray[i].Dir*pPartArray[i].Speed;
			//重力
			pPartArray[i].Dir+=D3DXVECTOR3(0,-0.0000098,0);
			//地面でのバウンド
			if(pPartArray[i].Pos.y<0)
			{
				pPartArray[i].Dir.y=-pPartArray[i].Dir.y;
			}
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
