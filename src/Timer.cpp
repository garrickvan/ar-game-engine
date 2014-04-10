/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "Timer.h"

EGTimer* EGTimer::m_pTimer=NULL;

EGTimer::EGTimer(){m_pTimer=this;}
EGTimer::~EGTimer(){}

/**\brief
 * 计时器的初始化
 *
 * 如果系统有高分辨率计时器，将保存tick频率，返回TRUE。
 * 否则函数返回FALSE,不能使用该计时器。
 */
BOOL EGTimer::Init()
{
	if (!QueryPerformanceFrequency(&m_ticksPerSecond))
	{
		return FALSE;// 系统不支持高分辨率计时器
	}

	else
	{
		QueryPerformanceCounter(&m_startTime);
		return TRUE;
	}
}



/**\brief
 * 获得经过的时间
 */
float EGTimer::GetElapsedSeconds()
{
	static LARGE_INTEGER s_lastTime = m_startTime;
	LARGE_INTEGER currentTime;

	QueryPerformanceCounter(&currentTime);

	float seconds=(float)(currentTime.QuadPart-s_lastTime.QuadPart)/(float)m_ticksPerSecond.QuadPart;

	// 记时器复位
	s_lastTime = currentTime;

	return seconds;
}


/**\brief
 * 获取帧率
 */
float EGTimer::GetFPS(unsigned long elapsedFrames)
{
	static LARGE_INTEGER s_lastTime = m_startTime;
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	float fps = (float)elapsedFrames * (float)m_ticksPerSecond.QuadPart / ((float)currentTime.QuadPart - (float)s_lastTime.QuadPart);
	// 重置定时器
	s_lastTime = currentTime;
	return fps;
} 


/**\beief
 * 锁定帧率
 */
float EGTimer::LockFPS(unsigned char targetFPS)
{
	if (targetFPS <= 0)targetFPS = 1;

	static LARGE_INTEGER s_lastTime = m_pTimer->m_startTime;
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	float   fps = m_pTimer->GetFPS();

	// 延时以保证固定的帧率
	while (fps > (float)targetFPS)
	{
		Sleep(1);
		QueryPerformanceCounter(&currentTime);
		fps=(float)m_pTimer->m_ticksPerSecond.QuadPart/((float)(currentTime.QuadPart-s_lastTime.QuadPart));
	}
	// 重置定时器
	s_lastTime = currentTime;

	return fps;
}
