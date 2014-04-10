/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#include "Window.h"

/** 显示AR跟踪的小窗口 */
class VisionView :	public EGWindow
{
public:
	VisionView(void);
	~VisionView(void);
	
	/**\brief
	 * 从INI文件载入载入视频图像界面
	 */
	BOOL LoadIni(char* _filename,char* _index);


private:
	/**\brief
	 * 图像的绘制
	 */
	void OnDraw(void);
};

