/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "Water.h"
#include "Terrain.h"


CWater::CWater()
{
	delta = 0.0f;
}

CWater::~CWater()
{
}


BOOL CWater::Init()
{
	if(!texture.InitTexture("data/water.tga",FALSE,TRUE))
		return FALSE;
	return TRUE;
}


void CWater::Render()
{
	static const int map_w = EGTerrain::GetSysTerrain()->GetMapWidth();
	static const float water_y = 0.0f;
	float water_w = (map_w + 800) / 713.0f;

	glDisable(GL_FOG);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	texture.Bind(0);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f+delta,0.0f+delta);glVertex3f(-800.0f, water_y , -800.0f);
	glTexCoord2f(0.0f+delta,water_w+delta);glVertex3f(-800.0f, water_y , map_w + 800.0f);
	glTexCoord2f(water_w+delta,water_w+delta);glVertex3f(map_w + 800.0f , water_y , map_w + 800.0f);
	glTexCoord2f(0.0f+delta,water_w+delta);glVertex3f(map_w + 800.0f , water_y , -800.0f);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_FOG);

	delta += 0.001f;
	if(delta >= 1.0f)delta -= 1.0f;
}
