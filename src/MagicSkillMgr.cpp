/**
 *作者：范修盛
 *2011年10月30日
 */
#include "MagicSkillMgr.h"
#include "IniFile.h"


MagicSkillMgr::MagicSkillMgr(void)
	:triggerType(MANUAL),curr_skill_index(0),new_skill_index(0),m_magicSkills(NULL)
{}

MagicSkillMgr::~MagicSkillMgr(void)
{}

BOOL MagicSkillMgr::Init(char * filename, char * _index)
{
	EGCIniFile Ini;
	if(!Ini.Open(filename))	return FALSE;

	totalSkillNum = Ini.ReadInt(_index, "totalSkillNum");
	if(totalSkillNum <= 0) return FALSE;

	m_magicSkills = new MagicSkill[totalSkillNum];
	char skillindex[128];
	for(int i = 0; i < totalSkillNum; i++)
	{
		sprintf(skillindex, "skill%d",i);
		if(!m_magicSkills[i].Init(
			filename,
			Ini.ReadText(_index,skillindex)))
			return FALSE;
	}
	return TRUE;
}

void MagicSkillMgr::Render()
{
	for(int i = 0; i < totalSkillNum; i++)
		if(m_magicSkills[i].isRun())
			m_magicSkills[i].Render();
}

void MagicSkillMgr::SetCurrSkillIndex(int index)
{
	new_skill_index = index;
}

BOOL MagicSkillMgr::CanReduceLife()
{
	if(m_magicSkills[curr_skill_index].IsStartRelease())
	{
		if(triggerType == AITRIGGER && totalSkillNum > 1)
		{
			new_skill_index = rand() % totalSkillNum;
		}
		curr_skill_index = new_skill_index;
		if(curr_skill_index != 0) new_skill_index = 0;
	}
	return m_magicSkills[curr_skill_index].CanReduceLife();
}