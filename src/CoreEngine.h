/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _CORE_ENGINE_H_
#define _CORE_ENGINE_H_

#include "input.h"
#include "font.h"
#include "camera.h"
#include "timer.h"
#include "sound.h"
#include "music.h"
#include "StaticObjMgr.h"
#include "role.h"
#include "monstersMgr.h"
#include "SceneMgr.h"
#include "GUIMgr.h"
#include "Message.h"
#include "ARMgr.h"

/*! 引擎类*/
class CoreEngine
{
public:
	CoreEngine(void);
	~CoreEngine(void);

	BOOL Init(void);				// 初始化，包括opengl方面的初始化

	void SetProjection(int iFOVAngle,int iHeight);// 设置矩阵

	void Prepare(void);				// 渲染前的准备工作

	void Render(void);				// 3D渲染

	void SwapBuffer(void){::SwapBuffers(m_hDC);}// 交换缓冲区

	void Close(void);				// 关闭引擎

	void SetStop(BOOL stop){m_bStop = stop;};

	BOOL GetStop(){return m_bStop;};

	static CoreEngine * GetSysCoreEngine(){return m_cCoreEngine;};

private:

	static CoreEngine * m_cCoreEngine;

	BOOL InitObject(void);			// 游戏中的实例对象的初始化

	BOOL SetDCPixelFormat(void);	// 设置opengl象素格式


	HDC			m_hDC;		// 设备句柄

	HGLRC		m_hRC;		// 渲染句柄

	HZFont*		m_pFont;	// 字体

	EGInput*		m_pInput;	// 输入系统

	EGTimer *		m_pTimer;	// 计时器

	EGCamera		camera;		// 摄像机

	EGMusic		m_cMusic;	// 背景音乐

	SceneMgr	m_cSceneMgr;// 场景管理

	GUIMgr		m_cGUIMgr;	// 界面管理

	Message		m_cMessage;	// 消息处理

	ARMgr	m_arMgr; //视频处理

	BOOL	m_bStop;	//游戏暂停
};
#endif 
