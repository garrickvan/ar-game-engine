/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "time.h"
#include "Snow.h"
#include "IniFile.h"


BOOL EGSnow::Init()
{
	
	EGCIniFile Ini("ini/scene.ini");

	int  snow_num= Ini.ReadInt("snow", "snownum");
	m_width = Ini.ReadInt("snow", "width");
	m_height = Ini.ReadInt("snow", "height");
	m_depth = Ini.ReadInt("snow", "depth");
	top_of_camera = Ini.ReadInt("snow", "top_of_camera");
	char * texFile = Ini.ReadText("snow", "texFile");
	isBlend = Ini.ReadInt("snow","isBlend");

	if(EGParticle::Init(snow_num))
	{
		for(int loop = 0 ; loop < m_iMaxNum ; ++loop)
		{
			ResetPaticle(&pList_particle[loop]);
		}
		if(snow_texture.InitTexture(texFile , TRUE))
			return TRUE;
		else
			return FALSE;
	}
	else
		return FALSE;
	
}



void EGSnow::Render()
{
	glDisable(GL_FOG);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	drawingBoard.Update();
	snow_texture.Bind(0);
	
	if(isBlend)
	{
		glEnable(GL_BLEND);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER , 0.0f);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	}
	glColor3f(1.f,1.f,1.f);
	int i;
	for(i = 0 ; i < m_iMaxNum ; ++i)
	{
		drawingBoard.Render(pList_particle[i].m_pos , pList_particle[i].m_size);
	}
	for( i = 0 ; i < m_iMaxNum; ++i)
	{
		pList_particle[i].m_pos += pList_particle[i].m_velocity;
		pList_particle[i].m_velocity += pList_particle[i].m_gravity;
		if(pList_particle[i].m_pos.v[1] < -50.0f)
			ResetPaticle(&pList_particle[i]);
	}
	
	if(isBlend)
	{
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
	}

	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FOG);
}

#define FAND(x) ((float)((rand() - rand()) % (x)))


void EGSnow::ResetPaticle(PARTICLE *particle)
{
	float x , y , z;
	vCameraPos=EGRole::GetRolePointer()->GetPos();
	x = vCameraPos.x +FAND(80);
	y = vCameraPos.y + top_of_camera;
	z = vCameraPos.z - 80.0f + FAND(100)/10;
	particle->m_pos = vector3d(x , y , z);
	particle->m_velocity = vector3d(0.0f , 0.0f ,0.0f);
	particle->m_size = 0.2f;
	x = ((float)FAND(10)) / 1000.0f;
	y= ((float)(rand() % 50)) / 5000;
	if(y>0)y=-y;
	if(y==0) y =-0.001f;
	z = ((float)FAND(10)) / 1000.0f;
	particle->m_gravity  = vector3d(x , y , z);
}