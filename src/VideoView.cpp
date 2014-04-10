/**
 *作者：范修盛
 *2011年10月30日
 */
#include "VideoView.h"
#include "ARMgr.h"
#include "Font.h"

static const double m_scalef = 0.348;

VideoView::VideoView(void){}

VideoView::~VideoView(void){}

/**\brief
 * 从INI文件载入视图数据
 */
BOOL VideoView::LoadIni(char* _filename,char* _index)
{
	EGCIniFile Ini(_filename);

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
void VideoView::OnDraw(void)
{
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

/*
			char * str = new char [126];
			sprintf(str , "this object is %d\n",object[i].trans[0][1]);
			ShowMessage(str);
void VideoCapture::draw( int object, double trans[3][4] )
{
    double    gl_para[16];
    GLfloat   mat_ambient[]     = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash[]       = {0.0, 0.0, 1.0, 1.0};
    GLfloat   mat_flash_shiny[] = {50.0};
    GLfloat   light_position[]  = {100.0,-200.0,200.0,0.0};
    GLfloat   ambi[]            = {0.1, 0.1, 0.1, 0.1};
    GLfloat   lightZeroColor[]  = {0.9, 0.9, 0.9, 0.1};

    argDrawMode3D();
    argDraw3dCamera( 0, 0 );
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    
    /* load the camera transformation matrix 
    argConvGlpara(trans, gl_para);
    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixd( gl_para );

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambi);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightZeroColor);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);	
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMatrixMode(GL_MODELVIEW);

    switch( object ) {
      case 0:
        glTranslatef( 0.0, 0.0, 25.0 );
        glutSolidCube(50.0);
        break;
      case 1:
        glTranslatef( 0.0, 0.0, 40.0 );
        glutSolidSphere(40.0, 24, 24);
        break;
      case 2:
        glutSolidCone(25.0, 100.0, 20, 24);
        break;
      default:
        glTranslatef( 0.0, 0.0, 10.0 );
        glutSolidTorus(10.0, 40.0, 24, 24);
        break;
    }

    glDisable( GL_LIGHTING );
    glDisable( GL_DEPTH_TEST );
}*/