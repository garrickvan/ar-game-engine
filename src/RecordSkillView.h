/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#include "Window.h"

/** 显示AR跟踪的小窗口 */
class RecordSkillView :	public EGWindow
{
public:
	RecordSkillView(void);
	~RecordSkillView(void);
	
	/**\brief
	 * 从INI文件载入载入视频图像界面
	 */
	BOOL LoadIni(char* _filename,char* _index);

	void draw(void);

private:
	/**\brief
	 * 图像的绘制
	 */
	void OnDraw(void);

	float m_escapeTime;
};

