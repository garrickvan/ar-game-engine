/**
 *作者：范修盛
 *2011年10月30日
 */

#include "stdafx.h"
#include "ActiveObj.h"
#include "inifile.h"
#include "Timer.h"
#include "CoreEngine.h"


void EGActiveObj::SetObjectState(OBJ_STATE state)
{
	if(property->object_state != state)
	{
		property->old_state = property->object_state;
		property->object_state = state;
		property->ChangeCurrFrame();
	}
}


void EGActiveObj::OnRender(void)
{
	glPushMatrix();
	property->vPosition=m_sPos;
	glTranslatef(property->vPosition.x , property->vPosition.y , property->vPosition.z);
	glRotatef(property->yrot,0.0f,1.0f,0.0f);
    
	if(property->shadow)
	{
		float m[16];
		float lp[4]={100.0f,100.0f,0.0f,0.0f};
		glPushMatrix();
		glLoadIdentity();
		glRotatef(-property->yrot,0.0f,1.0f,0.0f);
		glGetFloatv(GL_MODELVIEW_MATRIX,m);
		vMatMult(m,lp);
		glTranslatef(-property->vPosition.x,-property->vPosition.y,-property->vPosition.z);
		glPopMatrix();
		md2.CastShadow(lp,property->curr_frame);
	}

	if(property->m_bSelected)
	{
		glColor3f(1.f,0.f,0.f);
		glBegin(GL_LINE_LOOP);
			glVertex3f(-property->rx,1,-property->ry);
			glVertex3f(-property->rx,1, property->ry);
			glVertex3f( property->rx,1, property->ry);
			glVertex3f( property->rx,1,-property->ry);
		glEnd();
	}
	glColor4f(property->m_sColor.r,property->m_sColor.g,property->m_sColor.b,property->m_sColor.a);
	md2.RenderFrame(property->curr_frame,property->next_frame,property->t);
	glPopMatrix();	
	
	/** 调试标记 **/
	glPointSize(5.0);
	glBegin(GL_POINTS);
		glColor3f(1.0,0.0,0.0);
		glVertex3f(property->vPosition.x,property->vPosition.y, property->vPosition.z);
	glEnd();
	
	if(!SYS_COEG->GetStop())
	{
		EndRender();
	}
}



BOOL EGActiveObj::Init(char *filename,char*texfilename,float scale,float h)
{
	if(!md2.Init(filename,texfilename,scale,h))
		return FALSE;

	return TRUE;
}


void EGActiveObj::vMatMult(float M[], float v[])
{
    float res[4];										
	res[0]=M[ 0]*v[0]+M[ 4]*v[1]+M[ 8]*v[2]+M[12]*v[3];
	res[1]=M[ 1]*v[0]+M[ 5]*v[1]+M[ 9]*v[2]+M[13]*v[3];
	res[2]=M[ 2]*v[0]+M[ 6]*v[1]+M[10]*v[2]+M[14]*v[3];
	res[3]=M[ 3]*v[0]+M[ 7]*v[1]+M[11]*v[2]+M[15]*v[3];
	v[0]=res[0];									
	v[1]=res[1];
	v[2]=res[2];
	v[3]=res[3];
}

void EGActiveObj::MoveToPos(vector3d pos)
{
 	property->bArrive = FALSE;
 	if(property->object_state != RUN)SetObjectState(RUN);
 	property->start_pos = property->curr_pos;
}

void EGActiveObj::FaceToPos(vector3d pos)
{
    vector2d orient;		//偏转角度
    float theta1 , theta2;

    vector2d dest;			//角色2d位置
 	dest.x = pos.x;
 	dest.y = pos.z;
    property->end_pos = dest;
 	orient = property->end_pos - property->curr_pos;
 	orient.normalize();
 	theta1 = property->orient.getradian();
 	theta2 = orient.getradian();
 	property->yrot += theta1 - theta2;
 	property->orient = orient;
}