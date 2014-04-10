/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "scenemgr.h"
#include "IniFile.h"
#include "mathlib.h"
#include "CoreEngine.h"


SceneMgr* SceneMgr::m_psceneMgr=NULL;

SceneMgr::SceneMgr(void):m_pScene(NULL),m_eSceneType(SCE_NULL)
{
	m_psceneMgr=this;
}


SceneMgr::~SceneMgr(void)
{
	_DELETE(m_pScene);
}


/**\brief
 * 初始化
 */
BOOL SceneMgr::Init(void)
{
	EGCIniFile Ini;
	if(!Ini.Open("ini/scene.ini"))
		return FALSE;

	Terrain.Load(Ini.ReadText("scene","scene"));
	if(!m_cStaticObj.LoadIni("ini/StaticObj.ini","StaticObj"))return FALSE;
	
	if(!m_cMonsters.LoadIni("ini/monsters.ini","monsters"))return FALSE;
	if(!role.Init())return FALSE;
	
	return TRUE;
}

/**\brief
 * 从INI载入场景数据
 */
BOOL SceneMgr::LoadIni(char* _filename,char* _index)
{
	if(!m_pScene)return FALSE;
	if(!m_pScene->LoadIni(_filename,_index))return FALSE;
	if(m_pScene->Init())return FALSE;

	return TRUE;
}


/**\brief
 * 设置当前场景
 */
void SceneMgr::SetScene(GAME_SCENE _scene)
{
	m_eSceneType=_scene;
	if(m_pScene)_DELETE(m_pScene);

	switch(m_eSceneType)
	{
	case SCE_AVI	:m_pScene=new AVIScene;break;
	case SCE_OPENAIR:m_pScene=new OpenAirScene;break;
	default			:m_pScene=NULL;break;
	}
}


/**\brief
 * 场景的渲染
 */
void SceneMgr::Render(void)
{
	static float time = 0.0f;
	time += SYS_TIMER->m_fTime;

	if(!m_pScene)return;

	m_pScene->Render();

	if(m_eSceneType==SCE_OPENAIR||m_eSceneType==SCE_ROOM)
	{
		Terrain.Render();

		if(SYS_MOUSE->ButtonDown(0)||SYS_MOUSE->ButtonDown(1))Pick();

		m_cStaticObj.Render();
		
		if(SYS_KEYBOARD->KeyDown(DIK_P) && time > .30f)
		{
			SYS_COEG->SetStop(!SYS_COEG->GetStop());
			time = 0.0f;
		}

		role.Render();
		m_cMonsters.Render();
	}
}


void SceneMgr::StartPick(void)
{
	GLint	viewport[4];
    EGSPoint2 p= SYS_MOUSE->GetMousePos();

	glGetIntegerv(GL_VIEWPORT, viewport);
	glSelectBuffer(512, selectBuff);

	glPushMatrix();
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluPickMatrix( (double)(p.x),(double)(/*viewport[3]-*/p.y),5.0,5.0,viewport);
	gluPerspective(45.0f, (GLfloat)(viewport[2]-viewport[0])/(GLfloat)(viewport[3]-viewport[1]), 0.001f, F3D_DEPTH);
	glMatrixMode(GL_MODELVIEW);
}


void SceneMgr::EndPick(void)
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
}


BOOL SceneMgr::PickModel(int &type,unsigned int &id)
{
	glRenderMode(GL_SELECT);
	glInitNames();
	glPushName(0);
	glPushMatrix();
	EGCamera::GetCameraPointer()->FrameMove();

	unsigned int modelnum = 0;
	unsigned int i=0;

	for(i = 0 ; i < m_cStaticObj.GetObjNum(); ++i)
	{
		glLoadName(modelnum++);
		m_cStaticObj.Render_Pick(i);
	}
	for(i = 0 ; i < m_cMonsters.GetMonsterNum();++i)
	{
		glLoadName(modelnum++);
		m_cMonsters.Render_Pick(i);
	}
	glLoadName(modelnum);
	role.Render_Pick();
	glPopMatrix();
	GLint hits = glRenderMode(GL_RENDER);

	if(hits > 0)
	{
		int choose = selectBuff[3];
		int depth = selectBuff[1];

		for(int i=0;i<hits;++i)
		{
			if(selectBuff[i*4 + 1] < (GLuint)depth)
			{
				choose = selectBuff[i*4 + 3];
				depth  = selectBuff[i*4 + 1];
			}
		}

		id = choose;
		if(id==m_cStaticObj.GetObjNum()+m_cMonsters.GetMonsterNum())
		{
			type = ROLE;
			return TRUE;
		}
		if(id >=m_cStaticObj.GetObjNum())
		{
			type = MONSTER;
			id = id-m_cStaticObj.GetObjNum();
			return TRUE;
		}

		type = HOUSE;
		return TRUE;
	}
	
	return FALSE;
}


void SceneMgr::Pick(void)
{
	StartPick();
	unsigned int id;
	int type;
	char buffer[16];

	/*除掉所有实体上的红框框
	ObjectInfo.GetRolePointer()->m_bSelected=FALSE;
	for(unsigned int i=0;i<ObjectInfo.GetMonstersNum();++i)
	{
		ObjectInfo.GetMonsterPointer(i)->m_bSelected=FALSE;
	}*/

	PickModel(type , id);
	sprintf(buffer,"AR互动引擎");
	role.SetPickState(type ,id);

	::SetWindowText(SYS_HWND,buffer);

	EndPick();
}