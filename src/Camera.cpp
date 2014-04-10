/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "Camera.h"
#include "mathlib.h"
#include "role.h"
#include "Terrain.h"

/** 调整镜头倾斜角度的速度 **/
static const float PITCHSPEED = 1.0f * 5;
/** 调整镜头左右旋转角度的速度 **/
static const float DIRECTIONSPEED = 1.0f;
/** 调整镜头前后距离的速度 **/
static const float DIRECTIONTOHEROSPEED = 1.0f;

// 正在使用的摄像机的指针
EGCamera* EGCamera::m_pCamera = NULL;


EGCamera::EGCamera(void):direction(0.0f),pitch(21.f),
				dist_to_role(80.0f),role_pos(vector3d(2.0f,0.4f,-0.5f))
{
	m_pCamera = this;
}



/**\brief
 * 更新摄像的参数，把它放到主循环中
 */
void EGCamera::Update(void)
{
	if(EGInput::GetSysInput()->GetMouse()->GetWheelState()>0)
	{
		pitch += PITCHSPEED;
		if(pitch>100.0f)	pitch=100.0f;
	}
	if(EGInput::GetSysInput()->GetMouse()->GetWheelState()<0)
	{
		pitch -= PITCHSPEED;
		if(pitch<-3.f)	pitch=-3.f;
	}

	EGSPoint2 p=SYS_MOUSE->GetMousePos();
	if(p.x <=0)
	{
		if(EGRole::GetRolePointer()->IsArrive())direction+=DIRECTIONSPEED;
	}

	if(p.x >=SYS_WIDTH-1)
	{
		if (EGRole::GetRolePointer()->IsArrive())direction-=DIRECTIONSPEED;
	}
	if(p.y>SYS_HEIGHT-5)
	{
		dist_to_role += DIRECTIONTOHEROSPEED ;
		if(dist_to_role>160.0f)dist_to_role=160.0f;
	}
	if(p.y<5)
	{
		dist_to_role -= DIRECTIONTOHEROSPEED;
		if(dist_to_role<40.0f)dist_to_role=40.0f;
	}

	role_pos=EGRole::GetRolePointer()->GetPos();
	position = role_pos ;
	position.z += dist_to_role;
	position.y = role_pos.y+15.0f;
    

	FrameMove();
}


/**\brief
 * 设置变幻矩阵
 */
void EGCamera::FrameMove(void)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glTranslatef(0.0f,0.0f,-dist_to_role);
	glRotatef(pitch,1.0f,0.0f,0.f);
	glRotatef(direction,0.0f,1.0f,0.0f);

    glTranslatef(0.0f,0.0f,dist_to_role);

	glTranslatef(-position.x,-position.y,-position.z);
}

