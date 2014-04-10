/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "mapview.h"

MapView::MapView(void)
{
}

MapView::~MapView(void)
{
}


/**\brief
 * 从INI文件载入小地图数据
 */
BOOL MapView::LoadIni(char* _filename,char* _index, char* _videoFilename,char* _videoIndex)
{
	EGCIniFile Ini(_filename);

	m_sRect.width=Ini.ReadInt(_index,"width");
	m_sRect.height=Ini.ReadInt(_index,"height");

	m_chBackTexFile =Ini.ReadText(_index,"BackTexFile");
	
	m_bBlend=TRUE;
	m_sColor=EGSColor(1.f,1.f,1.f,0.9f);

	/** 与VideoView.ini数据同步 **/
	EGCIniFile vIni(_videoFilename);

	m_sRect.x = SYS_WIDTH - m_sRect.width - vIni.ReadInt(_videoIndex,"width");
	m_sRect.y = SYS_HEIGHT - m_sRect.height;

	if(m_chBackTexFile==NULL)return FALSE;
	this->LoadTexture(m_chBackTexFile,1,m_bBlend);

	return TRUE;
}


/**\brief
 * 小地图的渲染
 */
void MapView::OnDraw(void)
{
	float x , y;
	float mapWidth;
	mapWidth = EGTerrain::GetSysTerrain()->GetMapWidth();

	vector3d pos=EGRole::GetRolePointer()->GetPos();
	x = (pos.x /mapWidth * m_sRect.width) + m_sRect.x;
	y = -(pos.z /mapWidth * ((float)m_sRect.height * 0.8f)) + m_sRect.y+m_sRect.height;
	glPointSize(5.0f);
		glColor3f(1.0f,1.0f,0.0f);
	glBegin(GL_POINTS);
	  glVertex2f(x,y);
	glEnd();

	for(unsigned int i=0;i<EGMonstersMgr::GetSysMonstersMgr()->GetMonsterNum();++i)
	{
		pos = EGMonstersMgr::GetSysMonstersMgr()->GetMonsterPos(i);
		x = (pos.x / mapWidth * m_sRect.width) +m_sRect.x ;
		y =-(pos.z / mapWidth * ((float)m_sRect.height * 0.8f))+m_sRect.y+m_sRect.height;
		if(EGMonstersMgr::GetSysMonstersMgr()->GetMonsterState(i) == DEATH)
			glColor3f(0.0f , 0.0f , 0.0f);
		else
			glColor3f(1.0f , 0.0f , 0.0f);
		glBegin(GL_POINTS);
		glVertex2f(x,y);
		glEnd();
	}
	glPointSize(1.0f);
}
