/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#include "Window.h"
#include "button.h"
/** 显示AR跟踪的小窗口 */
class StoryDialog :	public EGWindow
{
public:
	StoryDialog(void);
	~StoryDialog(void);

	/**\brief
	 * 获得按钮数目
	 */
	unsigned int GetButtonNum(){return m_nButtonNum;}
	/**\brief
	 * 获得指定的按钮
	 */
	EGCButton* getButton(unsigned int index)
	{
		if(m_nButtonNum >= index && index >=1)
			return & m_pButton[index - 1];
	}

	BOOL LoadIni(char* _filename,char* _index);

private:
	// 菜单上的按钮数目
	unsigned int m_nButtonNum;

	// 按钮指针
	EGCButton*	m_pButton;

	void OnDraw(void);

	float m_escapeTime;
};