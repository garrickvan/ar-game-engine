/**
 *作者：范修盛
 *2011年10月30日
 */
#include "VisionView.h"
#include "ARMgr.h"

static const double m_scalef = 0.26;

VisionView::VisionView(void){}

VisionView::~VisionView(void){}

/**\brief
 * 从INI文件载入视图数据
 */
BOOL VisionView::LoadIni(char* _filename,char* _index)
{
	EGIniFile Ini(_filename);

	m_sRect.width=Ini.ReadInt(_index,"width");
	m_sRect.height=Ini.ReadInt(_index,"height");

	m_chBackTexFile =Ini.ReadText(_index,"BackTexFile");
	
	m_bBlend=TRUE;
	m_sColor=EGSColor(1.f,1.f,1.f,0.9f);
	m_sRect.x = SYS_WIDTH - m_sRect.width;
	m_sRect.y = SYS_HEIGHT - m_sRect.height;

	if(m_chBackTexFile==NULL)return FALSE;
	this->LoadTexture(m_chBackTexFile,1,m_bBlend);

	return TRUE;
}

/**\brief
 * 绘制视图窗口
 */
void VisionView::OnDraw(void)
{
	//////////////装载摄像头图像作为纹理///////////////
	ARUint8 * m_Img = SYS_VIDEO->getImgFromCap();

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
		glTranslatef(-((SYS_WIDTH - 15)/m_scalef) , -((SYS_HEIGHT - 14)/m_scalef), 0.0);
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