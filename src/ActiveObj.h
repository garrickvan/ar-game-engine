/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _ACTIVE_OBJ_H_
#define _ACTIVE_OBJ_H_

#include "object.h"
#include "MD2File.h"
#include "info.h"


/**\brief
 * 游戏中的活动实体
 */
class EGActiveObj:public Object
{
public:
	EGActiveObj(void):m_bActive(TRUE),property(NULL){}
	virtual ~EGActiveObj(void){}

	BOOL Init(char*filename,char*texfilename,float scale,float h);

	void SetObjectState(OBJ_STATE state);
	OBJ_STATE GetObjectState();
	
	float GetLife(void);
	float GetSize(void);
	float GetMina(void);
	
	BOOL IsArrive(void);

	virtual void OnRender(void);
	virtual void EndRender(void);

	void SetActive(BOOL _active){m_bActive=_active;}
	BOOL GetActive(){return m_bActive;}

protected:
	BOOL			m_bActive;
	CMD2File		md2;
	SActiveObjPro*	property;
	void vMatMult(float M[16],float v[4]);
	void			MoveToPos(vector3d pos);
	void			FaceToPos(vector3d pos);

protected:
	struct MD2_ANIMATION
	{
		char m_cName[16];
		int m_iStartFrame;
		int m_iEndFrame;
	};
};




//////////////////////////////////////////////////////////
//---------------------inline函数实现---------------------

inline OBJ_STATE EGActiveObj::GetObjectState(void)
{
	return property->object_state;
}

inline float EGActiveObj::GetLife(void)
{
	return property->GetLife();
}

inline float EGActiveObj::GetMina(void)
{
	return property->GetMina();
}

inline float EGActiveObj::GetSize(void)
{
	return md2.GetSize();
}

inline BOOL EGActiveObj::IsArrive(void)
{
	return property->bArrive;
}

inline void EGActiveObj::EndRender(void)
{
	property->vPosition=m_sPos;
	property->Update();
	m_sPos=property->vPosition;
}


#endif
