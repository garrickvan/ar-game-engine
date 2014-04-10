/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#include "Window.h"

/** 显示AR跟踪的小窗口 */
class TestPic :	public EGWindow
{
public:

	TestPic(void);
	~TestPic(void);
	
	static TestPic * GetTestPic(){return m_p;};

private:
	/**\brief
	 * 图像的绘制
	 */
	static TestPic * m_p;
	
	void OnDraw(void);

	float m_escapeTime;
};

