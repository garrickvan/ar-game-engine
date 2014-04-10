/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "MonstersMgr.h"
#include "inifile.h"

EGMonstersMgr* EGMonstersMgr::m_pMonstersMgr=NULL;

EGMonstersMgr::EGMonstersMgr()
{
	m_pMonster = NULL;
	m_pMonstersMgr=this;
}


EGMonstersMgr::~EGMonstersMgr()
{
	if(m_pMonster != NULL)
	{
		delete [] m_pMonster;
		m_pMonster = NULL;
	}
}


BOOL EGMonstersMgr::LoadIni(char* filename,char* index)
{
	EGCIniFile Ini(filename);
	m_nNumMonsters =Ini.ReadInt(index,"MonsterNum");
	m_pMonster = new EGMonster[m_nNumMonsters];

	//初始化怪物
	char* strFile;
	char* strIndex;
	for(unsigned int i=0; i<m_nNumMonsters; ++i)
	{
		strFile = Ini.ReadText(index, i);
		strIndex = Ini.ReadData(index,i);
		m_pMonster[i].LoadIni(strFile, strIndex);
		_FREE(strFile);
		_FREE(strIndex);
	}

	return TRUE;
}


void EGMonstersMgr::SetShadow(BOOL b)
{
	for(unsigned int i = 0; i < m_nNumMonsters; ++i)
	{
		m_pMonster[i].SetShadow(b);
	}
}

