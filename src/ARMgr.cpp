/**
 *作者：范修盛
 *2011年10月30日
 */
#include "ARMgr.h"
#include "general.h"

ARMgr* ARMgr::m_pARMgr = NULL;

ARMgr::ARMgr(void)
{
	arMgrThread = 0;
	cameState = FALSE;
	m_pARMgr = this;
}

ARMgr::~ARMgr(void)
{}

/** 运行独立线程的静态函数 */
void ARMgr::runFun(ARMgr vm)
{
	for(;;)
	{
		if(vm.cameState)
		{
			if(!vm.GetVideoCapture()->getRunState())
				vm.GetVideoCapture()->run();
		}
		else
		{
			/*** 释放资源 **/
			if(vm.GetVideoCapture()->getRunState())
			{
				vm.GetVideoCapture()->cleanup();
			}
			/*** 减缓监听速度 **/
			Sleep(300);
		}
	}
}

/** 当前视频管理器负责线程的主运行函数 **/
void ARMgr::Init()
{
	m_pVideoCapture.LoadIni("ini\\ARParam.ini","Camera");
	m_pMotionAnalysis.init();

	if(!arMgrThread)
	{
		/** 创建线程,并马上执行监听摄像头调用 **/
		arMgrThread = CreateThread (NULL, 0, (LPTHREAD_START_ROUTINE)runFun, ARMgr::m_pARMgr, 0, 0);
	}
}