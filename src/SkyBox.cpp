/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "SkyBox.h"
#include <math.h>
#include "Role.h"
#include "Timer.h"


CSkyBox::CSkyBox():yrot(0.f)
{
}

CSkyBox::~CSkyBox()
{
}


/**\brief
 * 天空的初始化
 */
BOOL CSkyBox::Init()
{
	if(!texture.InitTexture("scene/cloud.bmp"))
		return FALSE;
	glGenTextures(1, &cameraTexture[0]);
	return TRUE;
}

#define IMG_BOX (EGRole::GetRolePointer()->GetRecordSkill())

/**\brief
 * 渲染天空
 */
void CSkyBox::Render()
{
	static BOOL chang = FALSE;
	static BOOL init = FALSE;
	static float time = 0.0f;
	time += SYS_TIMER->m_fTime;

	if(time >= 0.3f)
	{
		if(SYS_KEYBOARD->KeyDown(DIK_F5) && IMG_BOX->hasRecroded())
			chang = !chang;
		time = 0.0f;
	}

	EGCamera::GetCameraPointer()->GetCameraPos(&m_sCameraPos);
	if(chang)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, cameraTexture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, 640, 480, 0, GL_BGRA, GL_UNSIGNED_BYTE, IMG_BOX->returnImg());
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	}
	else
		texture.Bind(0);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(m_sCameraPos.x,-40,m_sCameraPos.z-EGCamera::GetCameraPointer()->GetDestToRole());
	glRotatef(yrot,0.f,1.f,0.f);
	glColor3f(1.f,1.f,1.f);
	glBegin(GL_TRIANGLE_FAN);
		glTexCoord2f(0.5,0.5);
		glVertex3f(0,300,0);
		glTexCoord2f(0,0);
		glVertex3f(-300,0, 300);

		glTexCoord2f(1,0);
		glVertex3f(-300,0,-300);

		glTexCoord2f(1,1);
		glVertex3f( 300,0,-300);

		glTexCoord2f(0,1);
		glVertex3f( 300,0, 300);

		glTexCoord2f(0,0);
		glVertex3f(-300,0, 300);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);

	yrot += 0.04f;
	if(yrot > 360.0f)yrot -=360.0f;
}
