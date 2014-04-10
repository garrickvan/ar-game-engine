/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _MAGIC_SKILL_H_
#define _MAGIC_SKILL_H_

#include "stdAfx.h"
#include "BaseMagic.h"
#include "sound.h"

enum{
	SKILL_RELEASE = 0,
	SKILL_RUN,
	SKILL_END
};

class MagicSkill
{
public:
	MagicSkill(void);
	~MagicSkill(void);
	
	BOOL Init(char * filename,char * index);
	void Render();
	BOOL isRun(){return m_isRun;};
	int GetDAM(){return DAM;};
	void Change(vector3d& dest);
	void Set(vector3d start, vector3d end);
	BOOL IsStartRelease(){return m_Magic.b_startRelease;};
	void SetRelease(){m_Magic.b_startRelease = TRUE;};
	float GetManaCost(){return m_Magic.m_pManaCost;};
	BOOL CanReduceLife();

private:
	struct Magic{
		BaseMagic * m_pBaseMagics;
		BOOL	b_startRelease;
		float m_pManaCost;
		float m_pReleaseTime;	//技能释放时间
	};
	Magic m_Magic;
	int sumnum;
	int DAM;
	vector3d * offsetRule;
	vector3d * endOffsetRule;
	BOOL m_isRun;
	float m_sumReleaseTime; //释放总时间
	char * skillName;
	char startPiontType;
	EGSound sound;
};
#endif