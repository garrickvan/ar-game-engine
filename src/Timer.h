/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _TIMER_H_
#define _TIMER_H_

#include "StdAfx.h"


/**\brief
 * 高分辨率计时器类
 */
class EGTimer
{
public:
	EGTimer();
	~EGTimer();

	// 经过的时间，单位：秒
	float	m_fTime;

	// 帧率
	float	m_fFPS;


	/**\brief
	 * 计时器的初始化
	 *
	 * 如果系统有高分辨率计时器，将保存tick频率，返回TRUE。
	 * 否则函数返回FALSE,不能使用该计时器。
	 */
	BOOL Init();


	/**\brief
	 * 获得经过的时间
	 */
	float GetElapsedSeconds();


	/**\brief
	 * 获取帧率
	 */
	float GetFPS(unsigned long elapsedFrames=1);


	/**\beief
	 * 锁定帧率
	 */
	static float LockFPS(unsigned char targetFPS);


	/**\brief
	 * 刷新计时器
	 */
	void Update(){m_fTime=GetElapsedSeconds();m_fFPS=GetFPS();}


	/**\brief
	 * 取得当前计时器的指针
	 */
	static EGTimer* GetSysTimer(){return m_pTimer;}


private:
	LARGE_INTEGER   m_startTime;
	LARGE_INTEGER   m_ticksPerSecond;

	static EGTimer*	m_pTimer;
};

#endif
