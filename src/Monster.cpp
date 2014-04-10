/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "Monster.h"
#include "mmsystem.h"
#include "timer.h"
#include "inifile.h"
#include "Font.h"
#include "Music.h"

EGMonster::EGMonster()
	:isDead(FALSE),tracking(FALSE)
{
	property = new SActiveObjPro;
	SActiveObjPro info;
	info.speed = 0.4f;
	info.vPosition.x = 80.0f;
	info.vPosition.y = 0.0f;
	info.vPosition.z = 250.0f;
	info.start_pos.x = 80.0f;
	info.start_pos.y = 250.0f;
	info.curr_pos = info.start_pos;
	info.t_add = 0.1f;
	info.object_state = STAND;
	info.curr_frame = STAND_START;
	info.next_frame = info.curr_frame + 1;
	info.yrot = 0.0f;
	info.t = 0.0f;
	info.m_sColor=EGSColor(1.f,1.f,1.f,1.f);
	info.life = 100;
	info.curr_life = 100;
	SetMonsterInfo(info);

	time_elapse = timeGetTime();
}


BOOL EGMonster::Init(char *modelname,char *texturename,float scale,float h)
{
	if(!EGActiveObj::Init(modelname,texturename,scale,h))return FALSE;
	return TRUE;
}


/**\brief
 * 从INI文件初始化一个怪物
 */
BOOL EGMonster::LoadIni(char* filename,char* index)
{
	EGCIniFile Ini(filename);

	SActiveObjPro info;
	info.speed		= PERSENT(Ini.ReadInt(index,"speed"));
	m_sPos.x= DISTANCE_TO_X_SIDE + (float) (rand() % (EGTerrain::GetSysTerrain()->GetMapWidth()- 2 * DISTANCE_TO_X_SIDE));
	m_sPos.y= 0.0f;
	m_sPos.z= DISTANCE_TO_Y_SIDE + (float) (rand() % (EGTerrain::GetSysTerrain()->GetMapWidth()- 2 * DISTANCE_TO_Y_SIDE)); 

	info.start_pos.x= m_sPos.x;
	info.start_pos.y= m_sPos.z;
	info.curr_pos	= info.start_pos;
	info.yrot		= (float)Ini.ReadInt(index,"direction");
	info.m_sColor	=EGSColor(1.f,1.f,1.f,1.f);
	info.life		= Ini.ReadInt(index,"life");
	info.curr_life	= info.life;
	info.shadow		= Ini.ReadInt(index,"Shadow");
	info.dist_min	= (float)Ini.ReadInt(index,"dist_min");
	info.dist_max	= (float)Ini.ReadInt(index,"dist_max");
	this->SetMonsterInfo(info);

	if(!this->Init(
			Ini.ReadText(index,"ModelFile"),Ini.ReadText(index,"TexFile"),
			PERSENT(Ini.ReadInt(index,"scale")),PERSENT(Ini.ReadInt(index,"h"))
		))return FALSE;
	if(!m_magicSkillMgr.Init(
		Ini.ReadText(index, "magicskillsini"),
		Ini.ReadText(index, "magicskills")))return FALSE;
	m_magicSkillMgr.SetTriggerType(AITRIGGER);
	sound.LoadSoundFX("sound\\dead01.wav");
	return TRUE;
}

void EGMonster::EndRender()
{
	vector3d v;
	vector2d v2;
	if(property->object_state != DEATH)
	{
		ObjectInfo.GetRolePointer()->GetObjectPos(&v);
		float d = property->vPosition.dist_sqr(v);
		v2.x = v.x;
		v2.y = v.z;
		vector2d dis_pos = property->curr_pos - v2;
		///////////随着时间各个属性值都有一定的增加////////////
		static float dtime=0.f;
		dtime-=SYS_TIMER->m_fTime;


		//当角色的距离大于dist_min并且小于dist_max时,怪物追赶角色
		if(d <= property->dist_min)
		{
			property->bArrive = TRUE;
		}
		else if(((property->rememberPain >0 && d > property->dist_min)
			|| (d>property->dist_min&&d<property->dist_max)) && GetLife() >= 0.3)
		{
			if(!m_magicSkillMgr.CurrentSkill()->isRun())
			{
				FaceToPos(v);
				MoveToPos(v);
			}
			
		}
		else if(GetLife()<0.3f && d < property->dist_max)
		{
			vector2d dis_pos = property->curr_pos - v2;
			dis_pos.normalize();
			float theta1,theta2;
			theta1 = property->orient.getradian();
			theta2 = dis_pos.getradian();
			property->yrot += theta1 - theta2;
			property->orient = dis_pos;
			property->bArrive = FALSE;
			if(property->object_state != RUN)SetObjectState(RUN);
			property->start_pos = property->curr_pos;
			property->end_pos = property->start_pos + property->orient * (d-property->dist_min);
		}
		else if(!tracking && dtime<=0.f && rand() % 3 == 1 && d > property->dist_max && !(GetLife()<0.3f))
		{
			vector2d dis_pos =v2 - property->curr_pos ;
			dis_pos.normalize();
			float theta1,theta2;
			theta1 = property->orient.getradian();
			theta2 = dis_pos.getradian();
			property->yrot += theta1 - theta2;
			property->orient = dis_pos;
			property->bArrive = FALSE;
			if(property->object_state != RUN)SetObjectState(RUN);
			property->start_pos = property->curr_pos;
			property->end_pos = property->start_pos + property->orient * 100;

			//char inf[80];
			//sprintf(inf,"star:x:%d y:%d,end:x:%d,y:%d.",property->start_pos.x,property->start_pos.y,property->end_pos.x,property->end_pos.y);
			//ShowMessage(inf);
			tracking = TRUE;
		}
		else if(tracking)
		{
			if(property->end_pos.dist(property->curr_pos) <= 2.001 && property->end_pos.dist(v2) <= property->dist_min)
			{
				tracking = FALSE;
			}
		}
		else
			property->bArrive = TRUE;
		
		//当角色的距离小于dist_min时攻击
		if(d <= property->dist_min)
		{
			if(!m_magicSkillMgr.CurrentSkill()->isRun()
				&& property->bArrive
				&& ObjectInfo.GetRolePointer()->object_state != DEATH)
			{
				m_magicSkillMgr.CurrentSkill()->Set(property->vPosition,v);
				if(property->object_state != ATTACK)
				{
					SetObjectState(ATTACK);
					property->rememberPain = 0;
				}
			}
		}

		/** 调整攻击目标 **/
		if(m_magicSkillMgr.CurrentSkill()->isRun())m_magicSkillMgr.CurrentSkill()->Change(v);
		if(m_magicSkillMgr.CanReduceLife())
		{
			ObjectInfo.GetRolePointer()->ReduceLife(m_magicSkillMgr.CurrentSkill()->GetDAM());
		}

		if(dtime<=0.f)
		{
			property->curr_life+=2;
			if(property->curr_life>=property->life)property->curr_life=property->life;
			dtime=1.f;
		}

		/** 如果目标逃脱就停止攻击 **/
		if(!m_magicSkillMgr.CurrentSkill()->isRun()
			&& property->object_state != RUN
			&& property->object_state == ATTACK)	SetObjectState(STAND);
	}
	else
	{
		if(EGMusic::GetSysMusic()->GetMusic() && !isDead)sound.Play();
		isDead = TRUE;
	}
	
	glPointSize(5.0);
	glBegin(GL_POINTS);
		glColor3f(0.0,0.0,1.0);
		glVertex3f(v.x,v.y+ObjectInfo.GetRolePointer()->ry,v.z);
	glEnd();

	EGActiveObj::EndRender();
}


void EGMonster::SetMonsterInfo(SActiveObjPro& info)
{
	memcpy(property,&info,sizeof(SActiveObjPro));
	ObjectInfo.PushMonster(property);
}

void EGMonster::Render_Pick()
{
	property->t_add =SYS_TIMER->m_fTime*12.f;

	glPushMatrix();
	glTranslatef(property->vPosition.x,property->vPosition.y,property->vPosition.z);
	glRotatef(property->yrot,0.0f,1.0f,0.0f);
	md2.RenderFrame(property->curr_frame,property->next_frame,property->t);
	glPopMatrix();
}


void EGMonster::SetSpeed(float s)
{
	property->speed =s;
}
