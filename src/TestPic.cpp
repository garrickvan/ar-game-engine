/**
 *×÷Õß£º·¶ÐÞÊ¢
 *2011Äê10ÔÂ30ÈÕ
 */
#include "TestPic.h"
#include "ARMgr.h"
#include "RecordSkill.h"
#include "Role.h"
#include "Font.h"

TestPic * TestPic::m_p = NULL;

TestPic::TestPic(void)
{
	m_sRect.width=160;
	m_sRect.height=120;

	m_bBlend=TRUE;
	m_sColor=EGSColor(1.f,1.f,1.f,.0f);
	m_sRect.x = SYS_WIDTH /2  - m_sRect.width /2;
	m_sRect.y = SYS_HEIGHT/2 - m_sRect.height/2;
	m_p = this;
}

TestPic::~TestPic(void)
{}

#define _IMG_ (EGRole::GetRolePointer()->GetRecordSkill()->returnImg())

void TestPic::OnDraw(void)
{
	static int w = 0;
	static int h = 0;

	if(EGRole::GetRolePointer()->GetRecordSkill()->hasRecroded())
	{
		/*** ÉãÏñÍ·Í¼ÏñÎÆÀí **/
		GLuint cameraTexture[1];
		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &cameraTexture[0]);
		glBindTexture(GL_TEXTURE_2D, cameraTexture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, m_sRect.width, m_sRect.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, _IMG_);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		glPushMatrix();
		glRotatef( 180.0, 0.0, 0.0, 1.0);
		glTranslatef(-(SYS_WIDTH - m_sRect.width / 2 + 111) , -(SYS_HEIGHT - m_sRect.height / 2 + 79), 0.0);
		glBegin(GL_QUADS);
		glColor4f(m_sColor.r,m_sColor.g,m_sColor.b,m_sColor.a);
			glTexCoord2f(0.0f,0.0f);
			glVertex2i(m_sRect.x,m_sRect.y);

			glTexCoord2f((float)1,0.0f);
			glVertex2i(m_sRect.x+m_sRect.width,m_sRect.y);

			glTexCoord2f((float)1,(float)1);
			glVertex2i(m_sRect.x+m_sRect.width,m_sRect.y+m_sRect.height);

			glTexCoord2f(0.0f,(float)1);
			glVertex2i(m_sRect.x,m_sRect.y+m_sRect.height);
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		glDeleteTextures(1,&cameraTexture[0]);
		
		char str[80];
		sprintf(str,"Í¼Æ¬ÏñËØ£ºw:%d, h%d, r:%d, g:%d, b:%d, a:%d.",w,h,_IMG_[w * h],_IMG_[w * h + 1],_IMG_[w * h + 2],_IMG_[w * h + 3]);
		SYS_FONT->Format(str);
		SYS_FONT->PrintAt(0,260);
		
		/*for(int y = 0; y < m_sRect.height; y++) {
			for(int x = 0; x < m_sRect.width; x++) {
				_IMG_[4 * (y * m_sRect.width + x)] = 255;
				_IMG_[4 * (y * m_sRect.width + x) + 1] = 0;
				_IMG_[4 * (y * m_sRect.width + x) + 2] = 0;
				_IMG_[4 * (y * m_sRect.width + x) + 3] = 0;
			}
		}*/
	}
}