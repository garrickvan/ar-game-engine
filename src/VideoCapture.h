/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma onces
#ifndef _VIDEO_CAPTURE_H_
#define _VIDEO_CAPTURE_H_
#include "stdAfx.h"
#include "AR/ar.h"
#include "AR/video.h"
#include <AR/gsub.h>
#include "IniFile.h"

class VideoCapture
{
public:
	VideoCapture(void);
	~VideoCapture(void);
	/** 打开摄像头并负责获取图像 */
	void run();

	/** 初始化摄像头**/
	BOOL LoadIni(char* _filename,char* _index);
	
	/** 从摄像头获取一帧图像 **/
	ARUint8 * getImgFromCap();
	/** 从摄像头获取一帧图像 **/
	//ARInt16 * getImgFromCap();

	/** 获取视频文件的宽和高 **/
	int getWidth();
	int getHeight();
		
	/** 检测运行状态 **/
	BOOL getRunState();

	/** 清除资源 **/
	void cleanup();

	/** 获取记录卡片信息的结构对象 **/
	OBJECT_T * getObjects();
	
	/** 低帧速摄像头 **/
	void setLowSpeedCamera();

	void UpThresh();

	int GetThresh();

	void DownThresh();

private:

	/** 负责获取摄像头图像很分析图像的主函数 **/
	void mainLoop();
};
#endif