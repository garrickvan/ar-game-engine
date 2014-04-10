/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "object.h"
#include "role.h"

Object::Object(void):m_sPos(vector3d(0.f,0.f,0.f)),rx(24.f),ry(20.f)
{
}


Object::~Object(void)
{
}


void Object::Render()
{
	vector3d v=EGRole::GetRolePointer()->GetPos();
	if((fabsf(m_sPos.x-v.x)<DEPTHOFFIELD)&&(fabsf(m_sPos.z-v.z)<DEPTHOFFIELD) )
	{
		OnRender();
	}
}
