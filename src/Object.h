/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef __CGW_OBJECT_H__
#define __CGW_OBJECT_H__

#include "mathlib.h"
#include "terrain.h"

/**\brief
 * 所有游戏实体类的基类
 */
class Object
{
public:
	Object(void);
	~Object(void);

	float	rx,ry;		// 包围盒的半径,分别是x,y上的分量

	void Render();

	void SetPos(vector3d v)		{m_sPos=v;m_sPos.y = EGTerrain::GetSysTerrain()->GetHeight(m_sPos.x ,m_sPos.z);}

	vector3d GetPos(void)		{return m_sPos;}

	void SetHeightAt(float _h)	{m_sPos.y=_h;}


private:
	virtual void OnRender(){}


protected:
	vector3d   m_sPos;

};

#endif
