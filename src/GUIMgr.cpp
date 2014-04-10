/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "guimgr.h"
#include "inifile.h"
#include "input.h"
#include "message.h"
#include "Timer.h"
#include "ARMgr.h"
#include "Music.h"
#include "CoreEngine.h"

GUIMgr* GUIMgr::m_pGUIMgr=NULL;
BOOL testState = FALSE;

GUIMgr::GUIMgr(void):m_bMainMenu(FALSE),m_fFadeAlpha(0.f),m_bVideoMenu(FALSE),m_bTopMenu(FALSE)
{
	m_pGUIMgr=this;
}

GUIMgr::~GUIMgr(void)
{}


/**\brief
 * 从INI文件载入光标数据
 */
BOOL GUIMgr::LoadCurIni(char* _filename,char* _index)
{
	if(!m_cCursor.LoadIni(_filename,_index))return FALSE;
	bottom.LoadIni("ini\\bottom.ini","bottom");

	m_cChatList.SetRect(EGSRect(147,65,731,65));
	m_cChatList.SetColor(EGSColor(0.8f,0.5f,0.6f,0.5f));
	m_cChatList.SetBorderWidth(0);
	m_cChatList.SetBlend(TRUE);
	m_cChatList.SetItemColor(EGSColor(1.f,1.f,1.f,0.f));

	m_cChatEdit.SetRect(EGSRect(298,41,553,53));
	m_cChatEdit.SetBlend(TRUE);
	m_cChatEdit.SetColor(EGSColor(1.f,1.f,1.f,0.f));
	m_cChatEdit.SetInputNamePos(EGSPoint2(3,130));
	m_cChatEdit.SetFontColor(EGSColor(1.f,1.f,1.f,1.f));
	m_cChatEdit.SetCaretColor(EGSColor(1.f,1.f,1.f,1.f));

    m_cVideoMenu.LoadIni("ini\\VideoMenu.ini","VideoMenu"); //摄像交互菜单初始化
	m_cMainMenu.LoadIni("ini\\MainMenu.ini","MainMenu");
	m_cTopMenu.LoadIni("ini\\topmenu.ini","menu");
	m_cSkillMenu.LoadIni("ini\\skillmenu.ini","skill");
	m_cSkillMenu.SetVisible(TRUE);

	m_cMapView.LoadIni("ini\\MapView.ini","map01","ini\\VideoView.ini","view01");
	m_cVideoView.LoadIni("ini\\VideoView.ini","view01");
	m_cRecordSkillView.LoadIni("ini\\VideoView.ini","view02");
	m_cStoryDialog.LoadIni("ini\\StoryDialog.ini","dialog");
	m_cVideoView.SetVisible(FALSE);
	m_cRecordSkillView.SetVisible(FALSE);
	m_cStoryDialog.SetVisible(FALSE);

	m_cRoleLife.LoadIni("ini\\life.ini","role");
	m_cMonsterLife.LoadIni("ini\\life.ini","monster");

	m_cPropertyWnd.LoadIni("ini\\property.ini","dialog");
	m_cBagWnd.LoadIni("ini\\bag.ini","dialog");
	m_cLifeTex.InitTexture("interface\\life.bmp",TRUE);
	m_cMagicTex.InitTexture("interface\\magic.bmp",TRUE);
	m_cVideoView.SetVisible(TRUE);
	m_cMapView.SetVisible(TRUE);
	ARMgr::GetSysARMgr()->setCameraState(TRUE);

	return TRUE;
}

/**\brief
 * 淡入淡出
 */
void GUIMgr::Fade(void)
{
	if(m_fFadeAlpha<=0.1f)return;

	int width=SYS_WIDTH;
	int height=SYS_HEIGHT;

	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER , 0.0f);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	static float dtime=0.f;
	dtime-=SYS_TIMER->m_fTime;
	if(dtime<=0.0f) { m_fFadeAlpha-=0.0003f;dtime=0.03f;}

	glColor4f(0.f,0.f,0.f,m_fFadeAlpha);
	glBegin(GL_QUADS);
		glVertex2i(0,0);
		glVertex2i(width,0);
		glVertex2i(width,height);
		glVertex2i(0,height);
	glEnd();

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
}


/**\brief
 * 界面的渲染
 */
void GUIMgr::Render(void)
{
	static char str[25];
	static char buf[64];
	static float life;
	BeginOrtho(EGInput::GetSysInput()->GetMouse()->m_iWidth,EGInput::GetSysInput()->GetMouse()->m_iHeight);
	//SYS_COEG->SetStop(TRUE);
	///////////////////////////////////////////////////////
	//----------------游戏主场景时的界面-------------------
	if(SceneMgr::GetSysSceneMgr()->GetScene()==SCE_OPENAIR||SceneMgr::GetSysSceneMgr()->GetScene()==SCE_ROOM)
	{
		//------------生命球和魔法球--------
		bottom.draw();
		Ball(m_cLifeTex,34.f,24.f,EGRole::GetRolePointer()->GetLife());
		Ball(m_cMagicTex,882.f,24.f,EGRole::GetRolePointer()->GetMina());

		//------------聊天面板--------------
		if(m_cChatEdit.IsFocus())
		{
			m_cChatList.draw();
		}
		m_cChatEdit.draw();
		if(m_cChatEdit.IsFocus()&&m_cChatEdit.GetReturnState())
		{
			char* str=m_cChatEdit.GetText();
			if(strlen(str)>0)
			{
				m_cChatList.InsertItem(-1,str);
				m_cChatList.GetScrollBar()->SetPos(1.f);
			}
			m_cChatEdit.SetReturnState(FALSE);
		}
		if(m_cChatList.GetItemNum()>5)m_cChatList.DeleteItem(0);
		//----------------------------------

		//--------------血槽----------------
		m_cRoleLife.SetPos(EGRole::GetRolePointer()->stamina);
		m_cRoleLife.draw();

		//--------------提示的显示----------
		if(m_cRoleLife.IsActive())
		{
			SYS_FONT->SetColor(EGSColor(1.f,1.f,1.f,1.f));
			sprintf(str,"体力值：%.2f",EGRole::GetRolePointer()->stamina);
			SYS_FONT->Format(str);
			SYS_FONT->PrintAt(195,28);
		}
		EGSRect r;
		r=EGSRect(103,13,90,120);
		if(r.PointIn(SYS_MOUSE->GetMousePos()))
		{
			SYS_FONT->SetColor(EGSColor(1.f,1.f,1.f,1.f));
			sprintf(str,"生命值：%.2f",EGRole::GetRolePointer()->GetLife());
			SYS_FONT->Format(str);
			SYS_FONT->PrintAt(25,147);
		}
		r=EGSRect(880,10,81,90);
		if(r.PointIn(SYS_MOUSE->GetMousePos()))
		{
			SYS_FONT->SetColor(EGSColor(1.f,1.f,1.f,1.f));
			sprintf(str,"魔法值：%.2f",EGRole::GetRolePointer()->GetMina());
			SYS_FONT->Format(str);
			SYS_FONT->PrintAt(880,147);
		}
		//----------------------------------

		if(EGRole::GetRolePointer()->GetMonsterInfo(buf,life))
		{
			m_cMonsterLife.SetPos(life);
			m_cMonsterLife.draw();
			SYS_FONT->Format(buf);
			SYS_FONT->PrintAt(m_cMonsterLife.GetRect().x + 12, m_cMonsterLife.GetRect().y + 12);
		}
		
		//-------------顶部菜单-------------
		m_cTopMenu.draw();
		if(m_cTopMenu.GetState()==3)
		{
			m_bVideoMenu=TRUE;
			m_bMainMenu=FALSE;
		}
		if(m_cTopMenu.GetState()==5)
		{
			m_bVideoMenu=FALSE;
			m_bMainMenu=TRUE;
		}
		if(m_cTopMenu.GetState()==6)
		{
		   m_bTopMenu = !m_bTopMenu;
		   m_cTopMenu.ClearState();
		}
		if(m_bTopMenu)
		{
			m_cMapView.SetVisible(!m_cMapView.GetVisible());
			m_cTopMenu.ClearState();
		}
		m_cMapView.draw();

		//--------------摄像交互菜单的弹出--------------------
		if(m_bVideoMenu)
		{
			if(m_cVideoView.GetVisible())
			{
				m_cVideoMenu.getButton(2)->SetText("关 闭 影 像");
				ARMgr::GetSysARMgr()->setCameraState(TRUE);
			}
			else
			{
				m_cVideoMenu.getButton(2)->SetText("打 开 影 像");
			}

			m_cVideoMenu.draw();
			if(m_cVideoMenu.GetState()==1)						//摄像头开启
			{	
				testState = !ARMgr::GetSysARMgr()->getCameraState();
				
				ARMgr::GetSysARMgr()->setCameraState(testState);
				if(testState)
				{
					m_cVideoView.SetVisible(TRUE);
					m_cVideoMenu.getButton(1)->SetText("关 闭 摄 像 头");
				}
				else
				{
					m_cVideoView.SetVisible(FALSE);
					m_cVideoMenu.getButton(1)->SetText("打 开 摄 像 头");
				}
			}
			if(m_cVideoMenu.GetState()==2)						//影像开启
			{
				m_cVideoView.SetVisible(!m_cVideoView.GetVisible());
			}	
			if(m_cVideoMenu.GetState()==3)
			{
				//m_cStoryDialog.SetVisible(!m_cStoryDialog.GetVisible());
				m_cRecordSkillView.SetVisible(!m_cRecordSkillView.GetVisible());
				if(m_cRecordSkillView.GetVisible())
				{
					m_bVideoMenu = FALSE;
				}
			};  
			if(m_cVideoMenu.GetState()==4)
			{
				SYS_VIDEO->setLowSpeedCamera();
			}
			if(m_cVideoMenu.GetState()==5)	m_bVideoMenu=FALSE; //退出设置
			if(m_cVideoMenu.GetState()!=0)	m_cTopMenu.ClearState();
			m_cVideoMenu.ClearState();
		}

		//----------主菜单的弹出------------
		static BOOL t_bKeyDown1=FALSE;
		if(SYS_KEYBOARD->KeyDown(DIK_ESCAPE))t_bKeyDown1=TRUE;
		
		if(t_bKeyDown1&&!SYS_KEYBOARD->KeyDown(DIK_ESCAPE))
		{
			m_bMainMenu=!m_bMainMenu;
			t_bKeyDown1=FALSE;
		}
		if(m_bMainMenu)
		{
			m_cMainMenu.draw();
			if(m_cMainMenu.GetState()==1)	m_bMainMenu=FALSE;
			if(m_cMainMenu.GetState()==2)	EGMusic::GetSysMusic()->SetMusic(!EGMusic::GetSysMusic()->GetMusic());
			if(m_cMainMenu.GetState()==3)	SYS_COEG->SetStop(!SYS_COEG->GetStop());	//游戏暂停
			if(m_cMainMenu.GetState()==4)   //故事背景
			{		
				m_cStoryDialog.SetVisible(!m_cStoryDialog.GetVisible());
			}
			if(m_cMainMenu.GetState()==5)SYS_MESSAGE->Insert(SMessage(MSG_EXIT,0,0,NULL,NULL,NULL));
			if(m_cMainMenu.GetState()!=0)m_cTopMenu.ClearState();
			
			if(EGMusic::GetSysMusic()->GetMusic())
				m_cMainMenu.getButton(2)->SetText("关 闭 声 音");
			else
				m_cMainMenu.getButton(2)->SetText("打 开 声 音");
			if(SYS_COEG->GetStop())
				m_cMainMenu.getButton(3)->SetText("继 续 游 戏");
			else
				m_cMainMenu.getButton(3)->SetText("暂 停 游 戏");
			m_cMainMenu.ClearState();
		}
		//----------------------------------
		
		if(m_cTopMenu.GetState()==1)Property();          //人物属性栏
		if(m_cTopMenu.GetState()==2)Bag();               //物品栏

		////////////////////////技能栏////////////////////
		if(m_cTopMenu.GetState()==4)             
		{
			m_cSkillMenu.SetVisible(!m_cSkillMenu.GetVisible());
			m_cTopMenu.ClearState();
		}
		if(SYS_KEYBOARD->KeyDown(DIK_TAB) && m_cMapView.GetVisible())m_cMapView.draw(); 
		m_cSkillMenu.draw();
		if(true || SYS_ARML->getSkillMove()) MarkSkillMenu();
	}
	////////////////////////////////////////////////////
	//-----------帧率显示---------------
	if(SYS_KEYBOARD->KeyDown(DIK_F8))
	{
		char str[60];
		sprintf(str,"FPS:%.2f,Time:%.2f,cursor(%d,%d)",SYS_TIMER->m_fFPS,SYS_TIMER->m_fTime,SYS_MOUSE->GetMousePos().x,SYS_MOUSE->GetMousePos().y);
		SYS_FONT->Format(str);
		SYS_FONT->PrintAt(250,SYS_HEIGHT-20);
	}
	//----------------------------------

	//----------光标的绘制--------------
	if(SceneMgr::GetSysSceneMgr()->GetScene()!=SCE_AVI)
	{
		EGSPoint2 p=SYS_MOUSE->GetMousePos();
		m_cCursor.rect.x=p.x;
		m_cCursor.rect.y=p.y-m_cCursor.rect.height;//(SYS_HEIGHT-p.y)-m_cCursor.rect.height;
		m_cCursor.draw();
	}
	//----------------------------------

	Fade();//用来控制淡入淡出的函数

	if(m_cChatEdit.IsActive()
		||(m_cTopMenu.GetState()==1&&m_cPropertyWnd.IsActive())
		||(m_cTopMenu.GetState()==2&&m_cBagWnd.IsActive())
		||(m_bMainMenu&&m_cMainMenu.IsActive())
		||(m_bVideoMenu&&m_cVideoMenu.IsActive())
		||m_cTopMenu.IsActive())
		EGRole::GetRolePointer()->SetActive(FALSE);
	else if(!EGRole::GetRolePointer()->GetActive())
		EGRole::GetRolePointer()->SetActive(TRUE);

	//-----------视觉控制窗口-----------
	if(SceneMgr::GetSysSceneMgr()->GetScene()!=SCE_AVI)
	{
		if(m_cVideoView.GetVisible())		m_cVideoView.draw();
		if(m_cRecordSkillView.GetVisible())	m_cRecordSkillView.draw();
		if(m_cStoryDialog.GetVisible())   m_cStoryDialog.draw();
		ARMgr::GetSysARMgr()->GetMotionAnalysis()->update();
	}
	
	EndOrtho();
}


/**\brief
 * 英雄的属性
 */
void GUIMgr::Property(void)
{
	EGWindow win(m_cPropertyWnd.GetRect());
	win.SetBlend(TRUE);
	win.SetColor(EGSColor(0.9f,0.5f,0.5f,0.5f));
	win.draw();
	m_cPropertyWnd.draw();
	int x=m_cPropertyWnd.GetRect().x;
	int y=m_cPropertyWnd.GetRect().y;
	SYS_FONT->SetColor(EGSColor(1.f,0.f,0.f,1.f));
	SYS_FONT->Format("经验");SYS_FONT->PrintAt(x+88,y+350);
	SYS_FONT->Format("力量");SYS_FONT->PrintAt(x+88,y+315);
	SYS_FONT->Format("敏捷");SYS_FONT->PrintAt(x+88,y+260);
	SYS_FONT->Format("体力");SYS_FONT->PrintAt(x+88,y+190);
	SYS_FONT->Format("法力");SYS_FONT->PrintAt(x+88,y+140);

	if(m_cPropertyWnd.GetState()==1)
	{
		m_cTopMenu.ClearState();
		m_cPropertyWnd.ClearState();
	}
}


/**\brief
 * 英雄的物品栏
 */
void GUIMgr::Bag(void)
{
	EGWindow win(m_cBagWnd.GetRect());
	win.SetBlend(TRUE);
	win.SetColor(EGSColor(0.9f,0.5f,0.5f,0.5f));
	win.draw();
	m_cBagWnd.draw();

	if(m_cBagWnd.GetState()==1)
	{
		m_cTopMenu.ClearState();
		m_cBagWnd.ClearState();
	}
}

/**\brief
 * 生命球和魔法球的绘制
 */
void GUIMgr::Ball(EGCTexture tex,float x,float y,float pos)
{
	int w=108,h=119;
	glEnable(GL_TEXTURE_2D);
	tex.Bind(0);
	glEnable(GL_ALPHA_TEST);
	glBegin(GL_QUADS);
		glTexCoord2f(0.f,0.f);glVertex2f(x,y);
		glTexCoord2f(1.f,0.f);glVertex2f(x+w,y);
		glTexCoord2f(1.f,pos);glVertex2f(x+w,(y+h*pos));
		glTexCoord2f(0.f,pos);glVertex2f(x,(y+h*pos));
	glEnd();
	glDisable(GL_ALPHA_TEST);
	glDisable(GL_TEXTURE_2D);
}

void GUIMgr::MarkSkillMenu()
{
	static BOOL debug = FALSE;
	static float time = .5f;
	time += SYS_TIMER->m_fTime;
	//---调试
	if(time >= .3f)
	{
		if(SYS_KEYBOARD->KeyDown(DIK_F1))
			m_cStoryDialog.SetVisible(!m_cStoryDialog.GetVisible());
		if(SYS_KEYBOARD->KeyDown(DIK_F2)) debug = !debug;
		if(SYS_KEYBOARD->KeyDown(DIK_F3)) SYS_VIDEO->UpThresh();
		if(SYS_KEYBOARD->KeyDown(DIK_F4)) SYS_VIDEO->DownThresh();
		time = 0.0f;
	}

	SkillSelect select;
	EGCButton * bt = m_cSkillMenu.getButton(1);
	EGSRect rect = bt->GetRect() ;
	rect.width += 6;
	rect.height += 6;
	
	OBJECT_T * objs = SYS_VIDEO->getObjects();
	SYS_ARML->getSkillSelect(select);

	if(m_cSkillMenu.GetVisible())
	{
		glLineWidth(5.0f);
		if(select.z == 1)
			glColor3f(1.0f,0.0f,0.0f);
		else
			glColor3f(0.0f,1.0f,0.0f);
		
		if(select.x == 1)	bt = m_cSkillMenu.getButton(2);
		if(select.x == 2)	bt = m_cSkillMenu.getButton(3);
		if(select.y == 1)	bt = m_cSkillMenu.getButton(1);
		if(select.y == 2)	bt = m_cSkillMenu.getButton(4);
		if(select.x == 0 && select.y == 0)
		{
			rect.x -= 3;
			rect.y -= 6 + rect.height;
		}
		else
		{
			rect.x = bt->GetRect().x - 3;
			rect.y = bt->GetRect().y - 3;
		}

		glBegin(GL_LINE_LOOP);
		glVertex2i(rect.x, rect.y);
		glVertex2i(rect.x + rect.width, rect.y);
		glVertex2i(rect.x + rect.width, rect.y + rect.height);
		glVertex2i(rect.x, rect.y + rect.height);
		glEnd();
	}
	glLineWidth(2.0f);
	
	if(debug)
	{
		char str[80];
		sprintf(str,"调试信息：");
		SYS_FONT->Format(str);
		SYS_FONT->PrintAt(0,220);
		sprintf(str,"曝光阀值：%d.",SYS_VIDEO->GetThresh());
		SYS_FONT->Format(str);
		SYS_FONT->PrintAt(0,200);
		sprintf(str,"方块一x:%.2f,y:%.2f,z:%.2f,可见:%2d,时间:%f.",objs[0].trans[0][3],objs[0].trans[1][3],objs[0].trans[2][3],objs[0].visible,SYS_ARML->getMoveTime());
		SYS_FONT->Format(str);
		SYS_FONT->PrintAt(0,180);
		sprintf(str,"方块二x:%.2f,y:%.2f,z:%.2f,可见:%2d,时间:%f.",objs[1].trans[0][3],objs[1].trans[1][3],objs[1].trans[2][3],objs[1].visible,SYS_ARML->getSkillTime());
		SYS_FONT->Format(str);
		SYS_FONT->PrintAt(0,160);
	}
}
