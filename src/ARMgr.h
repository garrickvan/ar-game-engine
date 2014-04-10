/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _ARMGR_H_
#define _ARMGR_H_
#include "VideoCapture.h"
#include "MotionAnalysis.h"

/**视觉交互管理类*/
class ARMgr
{
public:
	ARMgr(void);
	~ARMgr(void);


	/** 设置摄像头状态 **/
	void setCameraState(BOOL state)	{	cameState = state;}

	/** 获取摄像头状态 **/
	BOOL getCameraState(){	return cameState;	}
		
	/** 启动视频管理器线程 */
	void Init();

	/** 返回视频管理器 */
	static ARMgr * GetSysARMgr(){return m_pARMgr;};
	
	/** 返回摄像头捕捉接口 */
	VideoCapture * GetVideoCapture(){return &m_pVideoCapture;};

	/** 返回摄像头捕捉接口 */
	MotionAnalysis * GetMotionAnalysis(){return &m_pMotionAnalysis;};

private:

	BOOL cameState;
	
	static ARMgr * m_pARMgr;

	/** 帧分析，主要分析从摄像头获取图片数据代表的动作含义 */
	MotionAnalysis m_pMotionAnalysis;

	/** 帧捕捉，主要负责从摄像头获取图片数据 */
	VideoCapture m_pVideoCapture;

	/** 视觉交互管理系统线程 */
	HANDLE arMgrThread;

	/** 视觉交互管理系统线程ID **/
	LPDWORD * lpThreadId;

	/** 运行独立线程的静态函数 **/
	static void runFun(ARMgr vm);
};
#endif