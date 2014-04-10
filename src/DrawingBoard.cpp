/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "DrawingBoard.h"

void EGCDrawingBoard::Update()
{
	float m[16];
	glGetFloatv(GL_MODELVIEW_MATRIX , m);
	right.set(m[0],m[4],m[8]);
	up.set(m[1],m[5],m[9]);
}

void EGCDrawingBoard::Render(vector3d vpos , float m_iSize)
{
	glPushMatrix();
	glBegin(GL_TRIANGLE_STRIP);
	  glTexCoord2f(0.0f , 1.0f); 
	  glVertex3fv((vpos + (up - right)*(m_iSize) ).v);
	  glTexCoord2f(0.0f , 0.0f); 
	  glVertex3fv((vpos + (right + up)*(-m_iSize) ).v);
      glTexCoord2f(1.0f , 1.0f); 
	  glVertex3fv((vpos + (right + up)*(m_iSize) ).v);
	  glTexCoord2f(1.0f , 0.0f);
	  glVertex3fv((vpos + (right - up)*(m_iSize) ).v);
	glEnd();
	glPopMatrix();
}

