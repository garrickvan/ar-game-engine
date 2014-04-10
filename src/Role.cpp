/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "Role.h"
#include <fstream>
#include <string>
#include "IniFile.h"
#include "timer.h"
#include "message.h"
#include "MonstersMgr.h"
#include "ARMgr.h"
#include "Music.h"
#include "RecordSkill.h"
using namespace std;

EGRole* EGRole::m_pRole = NULL;

EGRole::EGRole(void):bRightMouseDown(TRUE),pick_state(EARTH),
	m_pMonster(NULL),stamina(1.f),offsety(0),b_monsterPick(FALSE),isDead(FALSE)
{
	m_pRole = this;
	property = new SActiveObjPro;
}


BOOL EGRole::Init(void)
{
	LoadIni("ini\\role.ini","role");
	
	return TRUE;
}


BOOL EGRole::LoadIni(char* _filename,char* _index)
{
	EGCIniFile Ini(_filename);
	if(! property)return FALSE;

	float t_scale=PERSENT(Ini.ReadInt(_index,"scale"));
	float t_h	 =PERSENT(Ini.ReadInt(_index,"h"));
	if(!EGActiveObj::Init(Ini.ReadText(_index,"ModelFile"),Ini.ReadText(_index,"TexFile"),t_scale,t_h))return FALSE;
	property->shadow=Ini.ReadInt(_index,"shadow");

	property->m_sColor.r=PERSENT(Ini.ReadInt(_index,"r"));
	property->m_sColor.g=PERSENT(Ini.ReadInt(_index,"g"));
	property->m_sColor.b=PERSENT(Ini.ReadInt(_index,"b"));
	property->m_sColor.a=PERSENT(Ini.ReadInt(_index,"a"));

	property->rx=PERSENT(Ini.ReadInt(_index,"rx"));
	property->ry=PERSENT(Ini.ReadInt(_index,"ry"));
	property->speed=PERSENT(Ini.ReadInt(_index,"speed"));
	m_sPos.x=(float)Ini.ReadInt(_index,"x");
	m_sPos.y=(float)Ini.ReadInt(_index,"y");
	m_sPos.z=(float)Ini.ReadInt(_index,"z");
	offsety=(float)Ini.ReadInt(_index, "offsety");

	property->start_pos.x = m_sPos.x;
	property->start_pos.y = m_sPos.z;
	property->curr_pos = property->start_pos;
	
	property->life=Ini.ReadInt(_index,"life");
	property->curr_life=Ini.ReadInt(_index,"curr_life");
	property->mina=Ini.ReadInt(_index,"mina");
	property->curr_mina=Ini.ReadInt(_index,"curr_mina");
	property->dist_min	= (float)Ini.ReadInt(_index,"dist_min");
	property->dist_max	= (float)Ini.ReadInt(_index,"dist_max");

	m_pMagicSkillMgr.Init(Ini.ReadText(_index,"magicskill"), _index);
	m_pMagicSkillMgr.SetTriggerType(AUTOAIM);
	
	//这是可以加挨打时发出的声音
	sound.LoadSoundFX("sound\\dead02.wav");
	ObjectInfo.PushRole(property);
	m_RecordSkill.init();
    
	return TRUE;
}

void EGRole::EndRender(void)
{
	vector3d vector;				//角色位置
	float speed = property->speed;	//角色移动速度

	///////走路////////////
    if(property->object_state==RUN)
	{
		if(IsNotRun==TRUE)
		{
			if(EGMusic::GetSysMusic()->GetMusic())run.LoadSoundFX("sound\\run.wav");
			if(EGMusic::GetSysMusic()->GetMusic())run.Play(1);
		}
		IsNotRun=FALSE;
	}
	else{run.Stop();IsNotRun=TRUE;}
	/////////////////////////
    
    if(!property->bArrive) //体力值减少...
    {
    	stamina-=0.0003f;
    	if(stamina<=0.f) stamina=0.f;
    }
    
    ///////////随着时间各个属性值有一定的增加////
    static float dtime=0.f;
    dtime -= SYS_TIMER->m_fTime;
    if(dtime<=0.f)
    {
    	stamina += 0.01f;
    	if(stamina >=1.0f) stamina=1.f;
		property->curr_mina += 0.004f;
		if(property->curr_life>= property->life)
		property->curr_life = property->life;
		property->curr_life += 20.0f;
		if(property->curr_mina >= property->mina) property->curr_mina=property->mina;
    	dtime=.1f;
    }
    

	////////拾取检测////////////////
    if(pick_state == MONSTER || autoAimToMonster())
	{
		b_monsterPick = TRUE;
		m_pMonster = ObjectInfo.GetMonsterPointer(pick_id);
		m_pMonster->m_bSelected=TRUE;
	}
	else
	{
		b_monsterPick = FALSE;
		m_pMonster = NULL;
	}

    ///////////////////////////////////
    if(property->object_state !=DEATH)
    {
    	if(m_bActive)
    	{
			/*点击鼠标移动英雄角色*/
			if(SYS_MOUSE->ButtonDown(0))
			{
    			vector = EGInput::GetSysInput()->GetMousePosition();
				if(fabs(property->vPosition.x - vector.x) > (property->rx /2)
					&&  fabs(property->vPosition.z - vector.z) > (property->ry / 2)) 
				{
					FaceToPos(vector);
					MoveToPos(vector);
				}
			}
			else
			{
				vector = property->vPosition;
				/** 释放技能 **/
				/**键盘移动**/
				if(SYS_KEYBOARD->KeyDown(DIK_W) || SYS_KEYBOARD->KeyDown(DIK_UP))	vector.z -= speed;
				if(SYS_KEYBOARD->KeyDown(DIK_S) || SYS_KEYBOARD->KeyDown(DIK_DOWN))	vector.z += speed;
				if(SYS_KEYBOARD->KeyDown(DIK_A) || SYS_KEYBOARD->KeyDown(DIK_LEFT))	vector.x -= speed;
				if(SYS_KEYBOARD->KeyDown(DIK_D) || SYS_KEYBOARD->KeyDown(DIK_RIGHT))	vector.x += speed;
				if(ARMgr::GetSysARMgr()->getCameraState() && SYS_ARML->getActiveMove())
				{
					SYS_ARML->getMovePos(vector);
				}
				if(SYS_KEYBOARD->KeyDown(DIK_1) && m_pMonster)m_pMagicSkillMgr.SetCurrSkillIndex(1);
				if(SYS_KEYBOARD->KeyDown(DIK_2))m_pMagicSkillMgr.SetCurrSkillIndex(2);
				if(SYS_KEYBOARD->KeyDown(DIK_3))m_pMagicSkillMgr.SetCurrSkillIndex(3);
				if(SYS_KEYBOARD->KeyDown(DIK_4))m_pMagicSkillMgr.SetCurrSkillIndex(4);
				if(ARMgr::GetSysARMgr()->getCameraState())
				{
					SkillSelect ss;
					SYS_ARML->getSkillSelect(ss);
					if(ss.z == 1)
					{
						if(ss.x == 1) m_pMagicSkillMgr.SetCurrSkillIndex(1);
						if(ss.x == 2) m_pMagicSkillMgr.SetCurrSkillIndex(2);
						if(ss.y == 1) m_pMagicSkillMgr.SetCurrSkillIndex(3);
						if(ss.y == 2) m_pMagicSkillMgr.SetCurrSkillIndex(4);
					}
				}

				if(vector.dist(property->vPosition) > EPSILON)
				{
					FaceToPos(vector);
					MoveToPos(vector);
				}
			}

			////////////////////攻击////////////////////
			if(property->object_state != RUN)
			{
				if(b_monsterPick 
					&& m_pMonster->object_state != DEATH 
					&& !m_pMagicSkillMgr.CurrentSkill()->IsStartRelease()
					&& property->curr_mina >= m_pMagicSkillMgr.CurrentSkill()->GetManaCost())
				{
					m_pMagicSkillMgr.CurrentSkill()->Set(property->vPosition, m_pMonster->vPosition);
				}
				if(m_pMonster != NULL)	m_pMagicSkillMgr.CurrentSkill()->Change(m_pMonster->vPosition);
				////////////////////攻击形态////////////////////
				if(m_pMagicSkillMgr.CurrentSkill()->isRun()
					&& !m_pMagicSkillMgr.CurrentSkill()->IsStartRelease()
					&& property->object_state != ATTACK)
				{
					if(property->curr_mina >= m_pMagicSkillMgr.CurrentSkill()->GetManaCost())
					{
						property->curr_mina -= m_pMagicSkillMgr.CurrentSkill()->GetManaCost();
					}
					property->SetObjectState(ATTACK);
					if(m_pMonster) FaceToPos(m_pMonster->vPosition);
					m_pMagicSkillMgr.CurrentSkill()->SetRelease();
				}
			}

    		///////////挨打//////////////////
			if(m_pMagicSkillMgr.CanReduceLife())
			{
				if(m_pMonster)
				{
					m_pMonster->ReduceLife(m_pMagicSkillMgr.CurrentSkill()->GetDAM());
					m_pMonster->rememberPain = 1;
				}
			}
    	}
		else
		{
			////////////////////站立形态////////////////////
			if(!m_pMagicSkillMgr.CurrentSkill()->isRun() 
				&& property->object_state != RUN
				&& property->object_state != STAND)	property->SetObjectState(STAND);
		}

		/** 修正模型的y轴偏移 **/
		m_sPos.y += offsety;
		EGActiveObj::EndRender();
		m_sPos.y -= offsety;
    }
	else
	{
		if(EGMusic::GetSysMusic()->GetMusic() && !isDead)sound.Play();	
		isDead = TRUE;
		if(dtime<=-4.f)
		{
			isDead = FALSE;
			property->object_state = STAND;
			m_bActive = TRUE;
		}	
	}
}


void EGRole::Render_Pick(void)
{
	property->t_add = SYS_TIMER->m_fTime*13.f;

	glPushMatrix();
	glTranslatef(property->vPosition.x,property->vPosition.y,property->vPosition.z);
	glRotatef(property->yrot,0.0f,1.0f,0.0f);
	md2.RenderFrame(property->curr_frame,property->next_frame,property->t);
	glPopMatrix();
}


#define SYS_MON (EGMonstersMgr::GetSysMonstersMgr())

BOOL EGRole::GetMonsterInfo(char * pInfo,float& t)
{
	if(b_monsterPick && pick_id >= 0 && pick_id < SYS_MON->GetMonsterNum())
	{
		sprintf(pInfo,"monster%d",pick_id);
		t = m_pMonster->GetLife();
		return TRUE;
	}
	return FALSE;
}


void EGRole::SetSpeed(float s)
{
	property->speed =s;
}

BOOL EGRole::autoAimToMonster()
{	
	for(int i = 0; i < SYS_MON->GetMonsterNum(); i++)
	{
		if(SYS_MON->GetMonsterState(i) != DEATH
			&& SYS_MON->GetMonsterPos(i).dist(property->vPosition) <= property->dist_max)
		{
			pick_id = i;
			b_monsterPick = TRUE;
			return TRUE;
		}
	}
	return FALSE;
}