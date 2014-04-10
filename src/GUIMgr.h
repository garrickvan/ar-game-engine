/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef __GUI_MGR_H__
#define __GUI_MGR_H__

#include "ListView.h"
#include "edit.h"
#include "dialog.h"
#include "animate.h"
#include "window.h"
#include "ListView.h"
#include "edit.h"
#include "mapview.h"
#include "progressbar.h"
#include "SceneMgr.h"
#include "VideoView.h"
#include "RecordSkillView.h"
#include "StoryDialog.h"
#include "TestPic.h"
#include "RecordSkill.h"

/**\brief
 * 界面管理类
 */
class GUIMgr
{
public:
	GUIMgr(void);
	~GUIMgr(void);
	
	static GUIMgr* GetSysGUIMgr(){return m_pGUIMgr;}

	/**\brief
	 * 从INI文件载入光标数据
	 */
	BOOL LoadCurIni(char* _filename,char* _index);


	/**\brief
	 * 界面的渲染
	 */
	void Render(void);


	/**\brief
	 * 淡入淡出
	 */
	void Fade(void);

	/**\brief
	 * 开始出现淡入淡出效果
	 */
	void BeginFade(void){m_fFadeAlpha=1.f;}


private:
	static GUIMgr* m_pGUIMgr;
	EGAnimate m_cCursor;
	EGWindow bottom;

	ListView	m_cChatList;

	// 聊天输入框
	Edit		m_cChatEdit;

	// 主菜单
	Dialog		m_cMainMenu;
    Dialog      m_cVideoMenu;
	// 小地图
	MapView		m_cMapView;
	BOOL		m_bTopMenu;
	
	//视图窗口
	VideoView	m_cVideoView;

	//技能设定窗
	RecordSkillView m_cRecordSkillView;

	//故事背景
	StoryDialog m_cStoryDialog;

	// 是否弹出主对话框
	BOOL		m_bMainMenu;
    BOOL        m_bVideoMenu;
	// 上面的小菜单
	Dialog		m_cTopMenu;
	
	// 十字技能菜单
	Dialog		m_cSkillMenu;

	// 用来控制淡入淡出的变量
	float		m_fFadeAlpha;

	/////////////////////////////////////////
	// 英雄的血槽
	EGProgressBar m_cRoleLife;

	// 怪物的血槽
	EGProgressBar m_cMonsterLife;

	// 英雄属性窗口
	Dialog		m_cPropertyWnd;
	/**\brief
	 * 英雄的属性
	 */
	void Property(void);

	// 英雄的物品栏窗口
	Dialog		m_cBagWnd;
	/**\brief
	 * 英雄的物品栏
	 */
	void Bag(void);

	/**\brief
	 * 人物对话框
	 */
	void Dialog(void);
	void MosDialog(void);

	/**\brief
	 * 技能图标的选择标定
	 */
	void MarkSkillMenu();

	BOOL m_bStop();
	///////////////////////////////////////////
	EGCTexture	m_cLifeTex;
	EGCTexture	m_cMagicTex;
	TestPic testPic;

	/**\brief
	 * 生命球和魔法球的绘制
	 */
	void Ball(EGCTexture tex,float x,float y,float pos);
};

#endif
