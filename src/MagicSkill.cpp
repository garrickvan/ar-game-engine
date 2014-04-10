/**
 *作者：范修盛
 *2011年10月30日
 */
#include "MagicSkill.h"
#include "IniFile.h"
#include "CoreEngine.h"

MagicSkill::MagicSkill(void)
	:skillName(NULL),m_isRun(FALSE),startPiontType('S')
{
	m_Magic.m_pBaseMagics = NULL;
	m_Magic.m_pManaCost = 0.0f;
	m_Magic.m_pReleaseTime = 0.0f;
	m_Magic.b_startRelease = FALSE;
}

MagicSkill::~MagicSkill(void)
{
	delete [] m_Magic.m_pBaseMagics;
	delete [] offsetRule;
	delete [] endOffsetRule;
	delete skillName;
}

BOOL MagicSkill::Init(char * filename,char * _index)
{
	EGCIniFile Ini;
	if(!Ini.Open(filename)) return FALSE;//初始化失败！

	char * texFile = Ini.ReadText(_index, "texFile");
	if(!sound.LoadSoundFX(Ini.ReadText(_index, "sound"))) return FALSE;
	float m_size = PERSENT(Ini.ReadInt(_index,"size"));
	sumnum = Ini.ReadInt(_index,"sumnum");
	if(sumnum <= 0) return FALSE;						//初始化失败！
	DAM = Ini.ReadInt(_index, "DAM");
	skillName = Ini.ReadText(_index,"skillname");
	m_sumReleaseTime = PERSENT(Ini.ReadInt(_index, "sumreleasetime"));

	EGSColor co;
	float speed = PERSENT(Ini.ReadInt(_index, "speed"));
	startPiontType=Ini.ReadText(_index,"startpionttype")[0];

	if(startPiontType != 'S' && startPiontType != 'E')
	{
		char str[128];
		sprintf(str, "初始化技能：%s的偏移方式错误！",skillName);
		ShowMessage(str);
		return FALSE;//初始化失败！
	}

	co.r=PERSENT(Ini.ReadInt(_index,"r"));
	co.g=PERSENT(Ini.ReadInt(_index,"g"));
	co.b=PERSENT(Ini.ReadInt(_index,"b"));
	co.a=PERSENT(Ini.ReadInt(_index,"a"));

	m_Magic.m_pBaseMagics = new BaseMagic[sumnum];
	offsetRule = new vector3d[sumnum];
	endOffsetRule = new vector3d[sumnum];
	char strRule[128];
	for(int i = 0; i < sumnum; i++)
	{
		sprintf(strRule, "offsetx%d",i);
		offsetRule[i].x = PERSENT(Ini.ReadInt(_index, strRule));
		sprintf(strRule, "offsety%d",i);
		offsetRule[i].y = PERSENT(Ini.ReadInt(_index, strRule));
		sprintf(strRule, "offsetz%d",i);
		offsetRule[i].z = PERSENT(Ini.ReadInt(_index, strRule));

		sprintf(strRule, "endoffsetx%d",i);
		endOffsetRule[i].x = PERSENT(Ini.ReadInt(_index, strRule));
		sprintf(strRule, "endoffsety%d",i);
		endOffsetRule[i].y = PERSENT(Ini.ReadInt(_index, strRule));
		sprintf(strRule, "endoffsetz%d",i);
		endOffsetRule[i].z = PERSENT(Ini.ReadInt(_index, strRule));

		m_Magic.m_pBaseMagics[i].Init(texFile, m_size);
		m_Magic.m_pBaseMagics[i].SetColor(co.r,co.g,co.b,co.a);
		m_Magic.m_pBaseMagics[i].SetSpeed(speed);
	}
	m_Magic.m_pManaCost = PERSENT(Ini.ReadInt(_index, "manacost"));

	return TRUE;
}

void MagicSkill::Set(vector3d start, vector3d end)
{
	if(!m_isRun)
	{
		if(startPiontType == 'E')	start = end;
		m_isRun = TRUE;
		m_Magic.m_pReleaseTime = 0.0f;
		m_Magic.b_startRelease = FALSE;
		if(EGMusic::GetSysMusic()->GetMusic())sound.Play();
		for(int i = 0; i < sumnum; i++)
		{
			m_Magic.m_pBaseMagics[i].Reset();
			m_Magic.m_pBaseMagics[i].Set(start+offsetRule[i],end + endOffsetRule[i]);
		}
	}
}


void MagicSkill::Change(vector3d & dest)
{
	if(m_isRun)
	{
		vector3d end = dest;
		for(int i = 0; i < sumnum; i++)
		{
			m_Magic.m_pBaseMagics[i].Change(end + endOffsetRule[i]);
		}
	}
}

void MagicSkill::Render()
{

	if(m_isRun)
	{
		for(int i = 0; i < sumnum; i++)
		{
			if(!SYS_COEG->GetStop())
			{
				m_Magic.m_pBaseMagics[i].Update();
			}
			m_Magic.m_pBaseMagics[i].Render();
		}
	}
	
	if(!SYS_COEG->GetStop())
	{
		m_Magic.m_pReleaseTime += SYS_TIMER->m_fTime;
		if(m_Magic.m_pReleaseTime > m_sumReleaseTime)
		{
			m_Magic.m_pReleaseTime = 0.0f;
			for(int i = 0; i < sumnum; i++)
			{
				m_Magic.m_pBaseMagics[i].Reset();
			}
			m_isRun = FALSE;
			m_Magic.b_startRelease = FALSE;
		}
	}
}

BOOL MagicSkill::CanReduceLife()
{
	if(m_Magic.m_pReleaseTime <= m_sumReleaseTime)
	{
		int sumHit = 0;
		for(int i = 0; i < sumnum; i++)
		{
			if(m_Magic.m_pBaseMagics[i].isHit()) sumHit++;
		}
		if(sumHit == sumnum)
		{
			for(int i = 0; i < sumnum; i++)
			{
				m_Magic.m_pBaseMagics[i].Reset();
			}
			m_isRun = FALSE;
			m_Magic.b_startRelease = FALSE;
			return TRUE;
		}
	}
	return FALSE;
}