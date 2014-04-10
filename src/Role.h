/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _GAME_ROLE_H_
#define _GAME_ROLE_H_

#include "mathlib.h"
#include "ActiveObj.h"
#include "input.h"
#include "BaseMagic.h"
#include "sound.h"
#include "font.h"
#include "message.h"
#include "MagicSkillMgr.h"
#include "RecordSkill.h"

extern CInfo ObjectInfo;


/**\brief
 * 游戏中的英雄
 */
class EGRole:public EGActiveObj
{
public:
	EGRole(void);
	~EGRole(void){_DELETE(property)}

	void SetSpeed(float s);

	BOOL GetMonsterInfo(char *pInfo,float& t);

	void SetShadow(BOOL b){property->shadow=b;}
	void SetSelecte(BOOL b)		{property->m_bSelected=b;}
	void SetPickState(int state,int id){pick_state=state;pick_id=id;}

	virtual void Render_Pick(void);

	virtual void EndRender(void);

	virtual void OnRender(void){EGActiveObj::OnRender();m_pMagicSkillMgr.Render();};

	BOOL Init(void);

	static EGRole* GetRolePointer(){return m_pRole;}
	
	BOOL LoadIni(char* _filename,char* _index);

	float stamina;//精力

	RecordSkill * GetRecordSkill(){return &m_RecordSkill;};

private:
	MD2_ANIMATION *m_pAnimations;
	int pick_state;
    
	int pick_id;

	static EGRole* m_pRole;
	SActiveObjPro* m_pMonster;
	BOOL b_monsterPick;
    
	BOOL bRightMouseDown;
	EGSound sound;
	EGSound run;
	BOOL IsNotRun;
	MagicSkillMgr m_pMagicSkillMgr;
	int offsety;

	BOOL isDead;
	
	RecordSkill m_RecordSkill;

	BOOL autoAimToMonster();	//当怪物进入射程内时英雄自动瞄准技能释放点
};

#endif
