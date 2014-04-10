/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef __GAME_APP_H__
#define __GAME_APP_H__

#include "CoreEngine.h"
#include "Ime.h"
#include "IniFile.h"


/*! 应用程序类 */
class GameApp  
{
public:
	GameApp(void);
	~GameApp(void);

	BOOL	Init(EGSWindowInfo _sWinInfo);	// 初始化程序，窗口、输入等

	/**\brief
	 * 创建窗口
	 *
	 *\param	_sWinInfo	窗口信息
	 *\return	BOOL		创建是否成功
	 */
	BOOL	CreateGLWindow(EGSWindowInfo _sWinInfo);

	/**\brief
	 * 窗口消息处理函数
	 */
	LRESULT WINAPI MessageHandler(HWND hWnd,UINT message,WPARAM  wParam, LPARAM lParam);

	void Run(void);				// 程序的主循环


private:
	EGSWindowInfo	m_sWinInfo;		// 窗口信息

	MSG			msg;			// 消息
	
	BOOL		m_bActive;		// 窗口是否处于活动状态

	CoreEngine	m_coreEngine;		// 游戏的引擎实例

	void KillGLWindow(void);	// 销毁窗口
};


#endif
