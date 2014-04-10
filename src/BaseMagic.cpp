/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "BaseMagic.h"
#include "Timer.h"
#include "Font.h"

const float BaseMagic::MIN_DISTANCE = 0.5f;
#define ENDOFFSETY 10

BaseMagic::BaseMagic()
	:m_isHit(FALSE)
{
	r= 1.0f;
	g = 1.0f;
	b = 1.0f;
}

BOOL BaseMagic::Init(char *filename , float size)
{
	if(!texture.InitTexture(filename , TRUE))
		return FALSE;
	m_iSize = size;
	return TRUE;
}

void BaseMagic::Reset()
{
	m_isHit = FALSE;
}

void BaseMagic::Set(vector3d start, vector3d end)
{
	end_pos = end;
	start_pos = start;
}

void BaseMagic::Update()
{
	vector3d orient = end_pos - start_pos;
	orient.normalize();
	start_pos += orient * speed ;
	if(end_pos.dist_sqr(start_pos) < MIN_DISTANCE)m_isHit = TRUE;
}

void BaseMagic::Render()
{

	if(m_isHit) return;
	glDisable(GL_FOG);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER,0.0f);

	texture.Bind(0);
	glColor4f(r,g,b,a);	
	drawingBoard.Update();
	drawingBoard.Render(start_pos,m_iSize);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_ALPHA_TEST);
	glEnable(GL_FOG);
}
