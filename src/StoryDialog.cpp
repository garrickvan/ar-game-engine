/**
 *作者：钟华庭
 *2011年10月30日
 */
#include "StoryDialog.h"
#include "Font.h"
#include "ARMgr.h"
#include "CoreEngine.h"

StoryDialog::StoryDialog(void):m_pButton(NULL),m_nButtonNum(0),m_escapeTime(12.0f){}

StoryDialog::~StoryDialog(void){}

BOOL StoryDialog::LoadIni(char* _filename,char* _index)
{
	EGCIniFile Ini(_filename);

	m_sRect.width=Ini.ReadInt(_index,"width");
	m_sRect.height=Ini.ReadInt(_index,"height");

	m_chBackTexFile =Ini.ReadText(_index,"BackTexFile");
	
	m_bBlend=TRUE;
	m_sColor=EGSColor(1.f,1.f,1.f,1.0f);
	m_sRect.x = Ini.ReadInt(_index,"x");
	m_sRect.y = Ini.ReadInt(_index,"y");
	m_sColor.r=PERSENT(Ini.ReadInt(_index,"r"));
	m_sColor.g=PERSENT(Ini.ReadInt(_index,"g"));
	m_sColor.b=PERSENT(Ini.ReadInt(_index,"b"));
	m_sColor.a=PERSENT(Ini.ReadInt(_index,"a"));
	if(m_chBackTexFile==NULL)return FALSE;
	this->LoadTexture(m_chBackTexFile,1,m_bBlend);

	m_nButtonNum=Ini.ReadInt(_index, "ButtonNum");
	m_pButton=new EGCButton[m_nButtonNum];

	//初始化按钮
	char* strFile;
	char* strIndex;
	for(unsigned int i=0; i<m_nButtonNum; ++i)
	{
		strFile = Ini.ReadText(_index, i);
		strIndex = Ini.ReadData(_index,i);
		m_pButton[i].LoadIni(strFile, strIndex);
		_FREE(strFile);
		_FREE(strIndex);
	}
	return TRUE;
}


char story[][256]={
	"通灵术士操纵的通灵之眼及通灵之卡",
	"乃通灵大陆的圣神力量源泉,也是通灵大陆到达现实大陆的法宝。",
	"获取一定数量的通灵法宝可以打开通灵大陆与现实大陆的传送门",
	"现在通灵大陆的妖魔正在蠢蠢欲动,窥视着现实大陆......",
	"它们正在猎杀通灵战士,收集足够数量的通灵宝物...",
	"魔物：小子乖乖的交出手上的法宝，否则我们就不客气了！",
	"英雄：通灵之神请赐予我力量吧，我是不会向你们这群小妖屈服的！"};

/**\brief
 * 绘制视图窗口
 */
void StoryDialog::OnDraw(void)
{
	EGSColor cc;	
	cc.r = 1.0f;
	cc.g = 0.0f;
	cc.b = 0.0f;
	cc.a = 1.0f;

	m_escapeTime -= SYS_TIMER->m_fTime;
	
	if(m_escapeTime >= 6.001f)
	{
		for(int i = 0; i < 5; i++)
		{
			SYS_FONT->SetColor(cc);
			SYS_FONT->Format(story[i]);
			SYS_FONT->PrintAt(200,100 - i * 20);
		}
	}
	else if(m_escapeTime >= 3.001f)
	{
		SYS_FONT->SetColor(cc);
		SYS_FONT->Format(story[5]);
		SYS_FONT->PrintAt(500,100);
	}
	else
	{
		cc.r = 0.0f;
		cc.b = 1.0f;
		cc.g = 1.0f;
		SYS_FONT->SetColor(cc);
		SYS_FONT->Format(story[6]);
		SYS_FONT->PrintAt(200,100);
	}
	
	
	if(m_escapeTime <= 0.001f)
	{
		SetVisible(FALSE);
		m_escapeTime = 12.0f;
		SYS_COEG->SetStop(FALSE);
		return;
	}
	else
	{
		SYS_COEG->SetStop(TRUE);
	}
	//绘制按钮
	for(unsigned int i=0;i<m_nButtonNum;++i)m_pButton[i].draw();

	for(unsigned int i=0;i<m_nButtonNum;++i)
	{
		if(m_pButton[i].GetState()==BUTTON_DOWNL)this->m_iState=-1*(i+1);
		if(m_pButton[i].GetState()==BUTTON_CLICK)this->m_iState=i+1;
	}
	if(m_pButton[0].GetState()==BUTTON_DOWNL)
	{
	    SetVisible(FALSE);
		m_escapeTime = 12.0f;
		SYS_COEG->SetStop(FALSE);
	}
}