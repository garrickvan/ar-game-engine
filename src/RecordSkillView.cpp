/**
 *作者：范修盛
 *2011年10月30日
 */
#include "RecordSkillView.h"
#include "ARMgr.h"
#include "Font.h"
#include "CoreEngine.h"
#include "RecordSkill.h"
#include "Role.h"

static const double m_scalef = .926f;

RecordSkillView::RecordSkillView(void)
	:m_escapeTime(0.0f){}

RecordSkillView::~RecordSkillView(void){}

/**\brief
 * 从INI文件载入视图数据
 */
BOOL RecordSkillView::LoadIni(char* _filename,char* _index)
{
	EGCIniFile Ini(_filename);

	m_sRect.width=Ini.ReadInt(_index,"width");
	m_sRect.height=Ini.ReadInt(_index,"height");

	m_chBackTexFile =Ini.ReadText(_index,"BackTexFile");
	
	m_bBlend=TRUE;
	m_sColor=EGSColor(1.f,1.f,1.f,1.0f);
	m_sRect.x = SYS_WIDTH /2  - m_sRect.width /2;
	m_sRect.y = SYS_HEIGHT/2 - m_sRect.height/2;

	if(m_chBackTexFile==NULL)return FALSE;
	this->LoadTexture(m_chBackTexFile,1,m_bBlend);

	return TRUE;
}

void RecordSkillView::draw(void)
{
	static char str[80];
	int t = 4 - m_escapeTime;

	EGSColor cc;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBegin(GL_QUADS);
	glColor4f(.1f,.1f,.1f,.4f);
		glVertex2i(0.0f,0.0f);
		glVertex2i(SYS_WIDTH,0.0f);
		glVertex2i(SYS_WIDTH,SYS_HEIGHT);
		glVertex2i(0.0f,SYS_HEIGHT);
	glEnd();
	glDisable(GL_BLEND);
	
	cc.r = 1.0f;
	cc.g = 1.0f;
	cc.b = 0.0f;
	cc.a = 1.0f;
	
	if(m_escapeTime >= 0.0001f && m_escapeTime <= 3.0001f) sprintf(str,"图片录制倒计时: %d.",t);
	else if(m_escapeTime > 3.0001f)	sprintf(str,"图片处理中.");

	SYS_FONT->SetColor(cc);
	SYS_FONT->Format(str);
	SYS_FONT->PrintAt(SYS_WIDTH /2 - SYS_FONT->GetWidth() / 2,SYS_HEIGHT - 60);

	if(m_escapeTime > 3.0001f) EGRole::GetRolePointer()->GetRecordSkill()->RecordSkillFromeCamera();

	EGWindow::draw();
}

/**\brief
 * 绘制视图窗口
 */
void RecordSkillView::OnDraw(void)
{
	if(m_escapeTime >= 4.0f)
	{
		SetVisible(FALSE);
		m_escapeTime = 0.0f;
		SYS_COEG->SetStop(FALSE);
		return;
	}
	else
	{
		SYS_COEG->SetStop(TRUE);
	}
	m_escapeTime += SYS_TIMER->m_fTime;

	//////////////装载摄像头图像作为纹理///////////////
	ARUint8 * m_Img = SYS_VIDEO->getImgFromCap();
	OBJECT_T * objs = SYS_VIDEO->getObjects();

	if(m_Img)
	{
		/*** 摄像头图像纹理 **/
		GLuint cameraTexture[1];
		EGSRect m_pRect;

		m_pRect.width =  SYS_VIDEO->getWidth();
		m_pRect.height =  SYS_VIDEO->getHeight();
		m_pRect.x = 0;
		m_pRect.y = 0;

		glEnable(GL_TEXTURE_2D);
		glGenTextures(1, &cameraTexture[0]);
		glBindTexture(GL_TEXTURE_2D, cameraTexture[0]);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, m_pRect.width, m_pRect.height, 0, GL_BGRA, GL_UNSIGNED_BYTE, m_Img);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		
		glPushMatrix();
		glScalef(m_scalef,m_scalef,1.0);
		glRotatef( 180.0, 0.0, 0.0, 1.0);
		glTranslatef(-((SYS_WIDTH - m_sRect.width / 2 + 111)/m_scalef) , -((SYS_HEIGHT - m_sRect.height / 2 + 79)/m_scalef), 0.0);
		glBegin(GL_QUADS);
		glColor4f(m_sColor.r,m_sColor.g,m_sColor.b,m_sColor.a);
			glTexCoord2f(0.0f,0.0f);
			glVertex2i(m_pRect.x,m_pRect.y);

			glTexCoord2f((float)1,0.0f);
			glVertex2i(m_pRect.x+m_pRect.width,m_pRect.y);

			glTexCoord2f((float)1,(float)1);
			glVertex2i(m_pRect.x+m_pRect.width,m_pRect.y+m_pRect.height);

			glTexCoord2f(0.0f,(float)1);
			glVertex2i(m_pRect.x,m_pRect.y+m_pRect.height);
		glEnd();
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		glDeleteTextures(1,&cameraTexture[0]);
	}
}