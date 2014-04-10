/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _MAGIC_SKILL_MGR_H_
#define _MAGIC_SKILL_MGR_H_

#include "MagicSkill.h"

enum
{
	AITRIGGER = 0,	//自动瞄准及释放技能
	AUTOAIM,		//自动瞄准手动释放
	MANUAL,			//手动瞄准及手动释放
};

/**\breif
 *为当前的对象管理技能的释放
 */
class MagicSkillMgr
{
public:
	MagicSkillMgr(void);
	~MagicSkillMgr(void);

	BOOL CanReduceLife();
	void SetTriggerType(int _type){triggerType = _type;};
	void SetCurrSkillIndex(int index);
	BOOL Init(char * filename, char * _index);
	void Render();
	MagicSkill * CurrentSkill(){return &m_magicSkills[curr_skill_index];};

private:
	MagicSkill * m_magicSkills;
	int totalSkillNum;
	int curr_skill_index,new_skill_index;
	int triggerType;
};
#endif