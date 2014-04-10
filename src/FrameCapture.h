/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#include "stdAfx.h"
#include "MotionAnalysis.h"


class FrameCapture
{
public:
	FrameCapture(void);
	~FrameCapture(void);
	/** 从摄像头获取一帧图像 */
	void run();

	/** 打开摄像头 */
	void openCamera(){cameraIsOpen = TRUE;};
	/** 关闭摄像头 **/
	void closeCamera(){cameraIsOpen = FALSE;};

	/** 打开分析图像 */
	void openView(){viewWindowIsOpen = TRUE;};
	/** 关闭分析图像 **/
	void closeView();

	/** 打开处理结果图像 **/
	void openResult(){showResult = TRUE;};
	/** 关闭处理结果图像 **/
	void closeResult(){showResult = FALSE;};
	BOOL showOrNot(){return showResult;};

private:
	
	/** 打开摄像头 **/
	BOOL cameraIsOpen;
	/** 打开影像窗口 **/
	BOOL viewWindowIsOpen;

	/** 是否绘制检测结果 **/
	BOOL showResult;
};
