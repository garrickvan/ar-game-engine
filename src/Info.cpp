#include "stdafx.h"
#include "Info.h"
#include "StaticObjMgr.h"
#include "Role.h"
#include "mathlib.h"

CInfo ObjectInfo;


SActiveObjPro::SActiveObjPro():object_state(STAND),old_state(STAND),yrot(.0f),rx(4.f),ry(4.f),
				curr_frame(STAND_START),next_frame(STAND_START+1),t(0.f),t_add(.2f),bArrive(TRUE),
				orient(vector2d(1.0f,0.f)),dist_min(0.f),dist_max(0.f),m_bSelected(FALSE),rememberPain(0){}

void SActiveObjPro::ChangeCurrFrame()
{
	switch(object_state)
	{
		case STAND:
			curr_frame = STAND_START;
			next_frame = STAND_END;
			break;
		case WALK:
			curr_frame = WALK_START;
			next_frame = curr_frame + 1;
			break;
		case RUN:
			curr_frame = RUN_START;
			next_frame = curr_frame + 1;
			break;
		case ATTACK:
			curr_frame = ATTACK_START;
			next_frame = curr_frame + 1;
			break;
		case DEATH:
			curr_frame = DEATH1_START;
			next_frame = curr_frame + 2;
			break;
		case PAIN:
			curr_frame = PAIN1_START;
			next_frame = curr_frame + 1;
			break;
		case JUMP:
			curr_frame = JUMP_START;
			next_frame = curr_frame+1;
			break;
	}
	t = 0.0f;
}

#define SYS_TR (EGTerrain::GetSysTerrain())

void SActiveObjPro::Update()
{
	static float map_x = (SYS_TR->GetCellWidth() * SYS_TR->GetWidth()) - DISTANCE_TO_X_SIDE;
	static float map_z = (SYS_TR->GetCellWidth() * SYS_TR->GetWidth()) - DISTANCE_TO_Y_SIDE;

	if((curr_life<=0) && (object_state != DEATH))
		SetObjectState(DEATH);

	if(object_state != DEATH && object_state != PAIN)
	{

		if(!bArrive)
		{
			if(curr_pos.x <= (float) DISTANCE_TO_X_SIDE){		curr_pos.x = (float) DISTANCE_TO_X_SIDE;bArrive = TRUE;}
			if(curr_pos.x >= map_x){		curr_pos.x = map_x;bArrive = TRUE;}
			if(curr_pos.y <= (float) DISTANCE_TO_Y_SIDE){		curr_pos.y = (float) DISTANCE_TO_Y_SIDE;bArrive = TRUE;}
			if(curr_pos.y >= map_z){		curr_pos.y = map_z;bArrive = TRUE;}

			curr_pos += orient * speed;
			if(start_pos.dist_sqr(curr_pos) >= start_pos.dist_sqr(end_pos))
			{
				curr_pos = end_pos;
				bArrive = TRUE;
			}
			for(unsigned int i=0;i<(EGStaticObjMgr::GetSysStaticObjMgr()->GetObjNum());++i)
			{
				StaticObj* pObj=EGStaticObjMgr::GetSysStaticObjMgr()->GetStaticObj(i);
				if(pObj->rx>0.f||pObj->ry>0.f)
					if( (fabsf(vPosition.x-pObj->GetPos().x)<fabsf(rx+2 * pObj->rx))
						&&(fabsf(vPosition.z-pObj->GetPos().z)<fabsf(ry+2 * pObj->ry)) )
					{
						bArrive = TRUE;
						continue;
					}					
			}
			vPosition.x = curr_pos.x;
			vPosition.z = curr_pos.y;
		}
		else
		{
			if(object_state != STAND && object_state != ATTACK && object_state != JUMP)
			{
				SetObjectState(STAND);
				ChangeCurrFrame();
				return;
			}
		}
	}
	vPosition.y = EGTerrain::GetSysTerrain()->GetHeight(vPosition.x,vPosition.z);

	/////////////////////////////////
	if(t < 1.0f)
	{
		if(t + t_add < 1.0f)t += t_add;
		else t = 1.0f;
	}
	else
	{
		t = 0.0f;
		curr_frame = next_frame;
		++next_frame;
		switch(object_state)
		{
		case STAND:
			if(next_frame >= STAND_END)
				next_frame = STAND_START;
			break;
		case WALK:
			if(next_frame >= WALK_END)
				next_frame = WALK_START;
			break;
		case RUN:
			if(next_frame >= RUN_END)
				next_frame = RUN_START;
			break;
		case ATTACK:
			if(next_frame >= ATTACK_END)
				SetObjectState(STAND);	//攻击完成后切换到STAND状态
			break;
		case DEATH:
			if(next_frame >= DEATH1_END)
				next_frame -= 1;
			break;
		case PAIN:
			if(next_frame >= PAIN1_END)
				SetObjectState(old_state);
			break;
		case JUMP:
			if(next_frame>=JUMP_END)
				SetObjectState(old_state);
		}
	}
}

