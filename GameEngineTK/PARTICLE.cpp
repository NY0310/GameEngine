#include "PARTICLE.h"

//
//
//

PARTICLE::PARTICLE(D3DXVECTOR3& EmitPos)
{
	
		m_EmitPos=EmitPos;

		pPartArray.Pos=EmitPos;
		pPartArray.Dir= D3DXVECTOR3(2*((float)rand()/(float)RAND_MAX)-1,(float)rand()/(float)RAND_MAX,2*((float)rand()/(float)RAND_MAX)-1);
		D3DXVec3Normalize(&pPartArray.Dir,&pPartArray.Dir);
		pPartArray.Speed=(5+((float)rand()/(float)RAND_MAX))*0.00001;
		pPartArray.BirthFrame=rand();
		cnt = 0;
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
		if(m_Frame>pPartArray.BirthFrame)
		{
			pPartArray.Pos+=pPartArray.Dir*pPartArray.Speed;
			//重力
			pPartArray.Dir+= D3DXVECTOR3(0,-0.0000098,0);
			//地面でのバウンド
			if(pPartArray.Pos.y<0)
			{
				pPartArray.Dir.y=-pPartArray.Dir.y;
			}
		}
	}

	cnt++;
	if (cnt > 240)
	{
		delete this;
	}

}
//
//
//
D3DXVECTOR3& PARTICLE::GetParticlePos()
{
	return pPartArray.Pos;
}
