#include "stdafx.h"
#include "Particle.h"
#include "time.h"


EGParticle::EGParticle()
{
	pList_particle = NULL;	
}

EGParticle::~EGParticle()
{
	if(pList_particle != NULL)
	{
		delete [] pList_particle;
		pList_particle = NULL;
	}
}


/**\brief
 * 粒子系统的初始化
 *
 *\param	_num	粒子数
 */
BOOL EGParticle::Init(int _num)
{
	m_iMaxNum = _num;
	pList_particle = new PARTICLE[m_iMaxNum];
	if(pList_particle == NULL)
		return FALSE;
	return TRUE;
}


/**\brief
 * 粒子的渲染
 */
void EGParticle::Render()
{	
}
